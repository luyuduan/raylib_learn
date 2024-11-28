/**
 * Brief: 汉诺塔演示 按钮组件控制
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-20     lyd          Create
 *
 */

/* standard reference */

/* public   reference */
#include <stdio.h>

#include "raygui.h"
#include "hs_anim.h"
#include "hs_button.h"

/* private  reference */

typedef struct demo_button_st
{
    char *text;
    button_cb_pf cb;
} demo_button_st;

typedef struct demo_button2_st
{
    demo_button_st but2[BTN2_SWITCH_BUTT];
} demo_button2_st;

typedef struct demo_button_key_st
{
    int key;
} demo_button_key_st;

void button_clicked_cb_next(void *data);
void button_clicked_cb_reset(void *data);

void button_clicked_cb_stop(void *data);
void button_clicked_cb_resume(void *data);

/**
 * @brief 初始化
 *
 * @param[in/out] hs_btn -
 * @param[in] x - 按钮组的右边界
 * @param[in] y - 按钮组的上边界
 *
 * @note
 * @see
 */
void hs_button_init(hs_button_st *hs_btn, hs_struct_st *st, void *demo)
{
    demo_button_st demo_button[HS_BTN_BUTT] = {
        {"Reset", button_clicked_cb_reset},
        {"Next", button_clicked_cb_next},
    };
    demo_button2_st demo_button2[HS_BTN2_BUTT] = {
        {{
            {"Resume", button_clicked_cb_resume},
            {"Stop", button_clicked_cb_stop},
        }},
    };

    demo_button_key_st demo_button_key[HS_BTN_BUTT] = {
        {KEY_Q},
        {KEY_S},
    };
    demo_button_key_st demo_button2_key[HS_BTN2_BUTT] = {
        {KEY_SPACE},
    };

    int width  = st->button_width;
    int height = st->button_height;
    for (int i = 0; i < HS_BTN_BUTT; i++)
    {
        button_st *btn = &hs_btn->btn[i];

        button_init(btn, demo_button[i].text);
        button_set_callback(btn, demo_button[i].cb, demo);
        button_set_size(btn, width, height);
        button_bind_key(btn, demo_button_key[i].key);
    }

    for (int i = 0; i < HS_BTN2_BUTT; i++)
    {
        button2_st *btn2 = &hs_btn->btn2[i];

        button2_init(btn2, demo_button2[i].but2[BTN2_SWITCH_DEFAULT].text);
        button2_set_callback(btn2, demo_button2[i].but2[BTN2_SWITCH_DEFAULT].cb,
                             demo);
        button2_set_size(btn2, width, height);
        button2_bind_key(btn2, demo_button2_key[i].key);

        button2_attach(btn2, demo_button2[i].but2[BTN2_SWITCH_ATTACH].text,
                       demo_button2[i].but2[BTN2_SWITCH_ATTACH].cb, demo);
    }

    /* 横向排列，与info上对齐，与底座右对齐 */
    int x_start = st->button_x;
    int y_start = st->button_y;
    int i       = 0;
    for (; i < HS_BTN_BUTT; i++)
    {
        button_st *btn = &hs_btn->btn[i];
        button_set_pos(btn, x_start + i * width, y_start);
    }

    x_start += i * width;
    for (i = 0; i < HS_BTN2_BUTT; i++)
    {
        button2_st *btn2 = &hs_btn->btn2[i];
        button2_set_pos(btn2, x_start + i * width, y_start);
    }

    hs_btn->tips = "Q - retset   S - next   Space - stop/resume ";

    hs_btn->disk_num         = st->disk_num;
    hs_btn->disk_num_set     = hs_btn->disk_num;
    hs_btn->edit_mode        = false;
    hs_btn->disk_num_set_min = st->disk_num_set_min;
    hs_btn->disk_num_set_max = st->disk_num_set_max;
}

void hs_button_update(hs_button_st *hs_btn)
{
    for (int i = 0; i < HS_BTN_BUTT; i++)
    {
        button_st *btn = &hs_btn->btn[i];
        button_update(btn);
    }
    for (int i = 0; i < HS_BTN2_BUTT; i++)
    {
        button2_st *btn2 = &hs_btn->btn2[i];
        button2_update(btn2);
    }
}

void hs_button_draw(hs_button_st *hs_btn)
{
    for (int i = 0; i < HS_BTN_BUTT; i++)
    {
        button_st *btn = &hs_btn->btn[i];
        button_draw(btn);
    }
    for (int i = 0; i < HS_BTN2_BUTT; i++)
    {
        button2_st *btn2 = &hs_btn->btn2[i];
        button2_draw(btn2);
    }

    /* 值的更新，在下一次刷新。如果输入完直接reset，值就得不到更新，所以还需要再判断
     */
    if (GuiValueBox((Rectangle){hs_btn->btn->rec.x - hs_btn->btn->rec.width,
                                hs_btn->btn->rec.y, hs_btn->btn->rec.width,
                                hs_btn->btn->rec.height},
                    "disk num", &hs_btn->disk_num_set, hs_btn->disk_num_set_min,
                    hs_btn->disk_num_set_max, hs_btn->edit_mode))
    {
        hs_btn->edit_mode = !hs_btn->edit_mode;
    }
}

void hs_button_reset(hs_button_st *hs_btn, hs_struct_st *st, void *demo)
{
    hs_button_init(hs_btn, st, demo);
}

void hs_button_btn2_switch_def(hs_button_st *hs_btn)
{
    button2_set_state(&hs_btn->btn2[HS_BTN2_STOP_RESUME], BTN2_SWITCH_DEFAULT);
}

void hs_button_btn2_switch_attach(hs_button_st *hs_btn)
{
    button2_set_state(&hs_btn->btn2[HS_BTN2_STOP_RESUME], BTN2_SWITCH_ATTACH);
}

void hs_button_del_callback(hs_button_st *hs_btn)
{
    button_del_callback(&hs_btn->btn[HS_BTN_NEXT]);
    button2_del_callback(&hs_btn->btn2[HS_BTN2_STOP_RESUME]);
}

int hs_button_get_disk_num_set(hs_button_st *hs_btn)
{
    if (hs_btn->disk_num_set < hs_btn->disk_num_set_min
        || hs_btn->disk_num_set > hs_btn->disk_num_set_max)
    {
        return hs_btn->disk_num;
    }
    return hs_btn->disk_num_set;
}
