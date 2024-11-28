/**
 * Brief:  过渡动画
 *      ref: https://easings.net/zh-cn#
 *           https://spicyyoghurt.com/tools/easing-functions
 *      注意其语句中的赋值运算会立即改变值
 *      如 a = x * (t /= d) * t + d 等效于  t /= d; a = x * t * t + d;
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-19     lyd          Create
 *
 */

/* standard reference */
#include <stdio.h>

/* public   reference */
#include "animation.h"

/* private  reference */

/* 生成 匀速运动的位移量 序列 */
static void constant_speed_style(Vector2 *step_buf, int len, Vector2 x)
{
    Vector2 rate = Vector2Scale(x, 1.0 / (len - 1));
    for (int i = 0; i < len; i++)
    {
        step_buf[i] = Vector2Scale(rate, i);
    }
}

/* 产生匀加速运动的位移量序列 */
static void gravity_style(Vector2 *step_buf, int len, Vector2 x)
{
    for (int i = 0; i < len; i++)
    {
        float rate  = 1.0 * i / (len - 1);
        step_buf[i] = Vector2Scale(x, rate * rate);
    }
}

float easeOutSine(float x)
{
    return sin((x * PI) / 2);
}

static void easeOutSine_style(Vector2 *step_buf, int len, Vector2 x)
{
    for (int i = 0; i < len; i++)
    {
        float rate  = 1.0 * i / (len - 1);
        float rate1 = easeOutSine(rate);
        step_buf[i] = Vector2Scale(x, rate1);
    }
}
/*
    变量 x 表示 0（动画开始）到 1（动画结束）范围内的值。
 */
float easeOutBounce(float x)
{
    float n1 = 7.5625;
    float d1 = 2.75;

    if (x < 1 / d1)
    {
        return n1 * x * x;
    }
    else if (x < 2 / d1)
    {
        x -= 1.5 / d1;
        return n1 * x * x + 0.75;
    }
    else if (x < 2.5 / d1)
    {
        x -= 2.25 / d1;
        return n1 * x * x + 0.9375;
    }
    else
    {
        x -= 2.625 / d1;
        return n1 * x * x + 0.984375;
    }
}

static void easeOutBounce_style(Vector2 *step_buf, int len, Vector2 x)
{
    for (int i = 0; i < len; i++)
    {
        float rate  = 1.0 * i / (len - 1);
        float rate1 = easeOutBounce(rate);
        step_buf[i] = Vector2Scale(x, rate1);
    }
}

typedef struct animation_st
{
    style_pf func;
} animation_st;

animation_st g_animation_library[ANI_BUTT] = {
    {constant_speed_style},
    {gravity_style},
    {easeOutSine_style},
    {easeOutBounce_style},
};

char *animation_style_name[] = {
    "const v",
    "const a",
    "OutSine",
    "OutBounce",
};

void animation_set_style(Vector2 *array, int len, Vector2 c,
                         animation_type_en style)
{
    if (style >= ANI_BUTT)
    {
        printf("no support style, %d, see 'enum animation_type_en'\n", style);
        return;
    }

    g_animation_library[style].func(array, len, c);
}

/**
 * @brief 初始化一次动画
 *
 * @param[in/out] f -
 * @param[in/out] b - 初始位置
 * @param[in/out] c - 增量
 * @param[in/out] style -
 *
 * @note    默认停止，可调用 animation_start 执行动画
 * @see
 */
void animation_init(ani_frame_st *f, Vector2 start, Vector2 end,
                    animation_type_en style)
{
#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

    animation_set_style(f->buf, ARRAY_LEN(f->buf), Vector2Subtract(end, start),
                        style);
    for (size_t i = 0; i < ARRAY_LEN(f->buf); i++)
    {
        f->buf[i] = Vector2Add(f->buf[i], start);
    }

    f->idx = 0;

    f->press_speed_up_key = -1;
    f->speed_up_key       = -1;
    f->speed_down_key     = -1;
    f->idx_det            = 1;
    f->idx_det_max        = (ARRAY_LEN(f->buf)) / 20 + 1;

    animation_stop(f); /* 默认停止 */
    f->if_run_once = false;
}

/**
 * @brief 帧更新
 *
 * @param[in/out] f -
 * @return true 动画完成
 * @return false
 *
 * @note
 * @see
 */
bool animation_update(ani_frame_st *f)
{
    if (f->if_stop)
    {
        return false;
    }

    if (f->if_run_once)
    {
        f->if_run_once = false;
        animation_stop(f);
    }

    if (IsKeyPressed(f->speed_up_key))
    {
        animation_set_speed(f, f->idx_det + 1);
    }
    if (IsKeyPressed(f->speed_down_key))
    {
        animation_set_speed(f, f->idx_det - 1);
    }

    if (IsKeyDown(f->press_speed_up_key))
    {
        f->idx += f->idx_det_max;
    }
    else
    {
        f->idx += f->idx_det;
    }

    if (f->idx >= (int)ARRAY_LEN(f->buf))
    {
        f->idx = ARRAY_LEN(f->buf) - 1;
        return true;
    }

    return false;
}

void animation_stop(ani_frame_st *f)
{
    f->if_stop = true;
}

void animation_start(ani_frame_st *f)
{
    f->if_stop     = false;
    f->if_run_once = false;
}

void animation_resume(ani_frame_st *f)
{
    animation_start(f);
}

void animation_set_speed_up_key(ani_frame_st *f, int key)
{
    f->press_speed_up_key = key;
}

void animation_set_speed_key(ani_frame_st *f, int up_key, int down_key)
{
    f->speed_up_key   = up_key;
    f->speed_down_key = down_key;
}

/**
 * @brief 设置动画速度
 *
 * @param[in] f -
 * @param[in] speed - 1-10
 *
 * @note
 * @see
 */
void animation_set_speed(ani_frame_st *f, int speed)
{
    f->idx_det = 1.0 * speed / 10 * f->idx_det_max;
    if (f->idx_det > f->idx_det_max)
    {
        f->idx_det = f->idx_det_max;
    }
    if (f->idx_det < 1)
    {
        f->idx_det = 1;
    }
}

void animation_next(ani_frame_st *f)
{
    if (f->if_stop)
    {
        f->if_run_once = true;
        f->if_stop     = false;
    }
}

void animation_reset(ani_frame_st *f, Vector2 start, Vector2 end,
                     animation_type_en style)
{
    animation_init(f, start, end, style);
}

Vector2 animation_x(ani_frame_st *f)
{
    return f->buf[f->idx];
}

bool animation_if_stop(ani_frame_st *f)
{
    return f->if_stop;
}
