/**
 * Brief: 过渡动画
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-19     lyd          Create
 *
 */

#ifndef __ANIMATION_H__
#define __ANIMATION_H__
#include "raylib.h"
#include "raymath.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define ANI_BUFF_LEN 60

    typedef enum animation_type_en
    {
        ANI_CONST_V,   /* 匀速 */
        ANI_CONST_A,   /* 匀加速 */
        ANI_OutSine,   /* 逐渐减速 */
        ANI_OutBounce, /* 弹跳 */
        ANI_BUTT
    } animation_type_en;

    typedef struct ani_frame_st
    {
        Vector2 buf[ANI_BUFF_LEN];
        int idx;

        /* 动画加速 */
        int press_speed_up_key; /* 按住加速，松开恢复 */
        int speed_up_key;       /* 按一次加一次速 */
        int speed_down_key;     /* 按一次减一次速 */
        int idx_det;            /* 当前速度 */
        int idx_det_max;        /* 最大速度 */

        bool if_stop;
        bool if_run_once;
    } ani_frame_st;

    extern char *animation_style_name[];
    typedef void (*style_pf)(Vector2 *array, int len, Vector2 x);

    void animation_init(ani_frame_st *f, Vector2 start, Vector2 end,
                        animation_type_en style);
    bool animation_update(ani_frame_st *f);
    void animation_stop(ani_frame_st *f);
    void animation_start(ani_frame_st *f);
    void animation_next(ani_frame_st *f);
    void animation_resume(ani_frame_st *f);
    void animation_set_speed_up_key(ani_frame_st *f, int key);
    void animation_set_speed_key(ani_frame_st *f, int up_key, int down_key);
    void animation_set_speed(ani_frame_st *f, int speed);
    void animation_reset(ani_frame_st *f, Vector2 start, Vector2 end,
                         animation_type_en style);
    Vector2 animation_x(ani_frame_st *f);
    bool animation_if_stop(ani_frame_st *f);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ANIMATION_H__ */
