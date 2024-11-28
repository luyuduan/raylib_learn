/**
 * Brief: 汉诺塔演示 动画组件
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-20     lyd          Create
 *
 */

#ifndef __HS_ANIMATION_H__
#define __HS_ANIMATION_H__
#include "hanoi.h"
#include "animation.h"
#include "hs_structure.h"
#include "hs_disk_map.h"
#include "hs_button.h"

#ifdef __cplusplus
extern "C"
{
#endif
#define TRACK_NUM 3

    /*
        圆盘移动过程

        (sour_top)   -->    (dest_top)

            ^                   |
            |                   |
            |                   v

        (sour    )          (dest    )

        act   ：表示从一柱移动到另一柱子的过程
        track ：表示三次直线移动的过程之一

     */
    typedef struct hs_act_st
    {
        int disk_num;

        record_st *record; /* 解法列表 */

        int record_len;
        int record_step; /* record的游标 */
        char info_str[32];

        column_disk_st sour;
        column_disk_st sour_top;
        column_disk_st dest_top;
        column_disk_st dest;
        column_disk_st *track_point[TRACK_NUM + 1];
        animation_type_en line_style[TRACK_NUM];
        int size;      /* 移动圆盘的尺寸 */
        int track_idx; /* 阶段游标 */

        ani_frame_st track;

        bool if_stop_act;
        bool if_run_once_act;

        hs_button_st *btn;

        int speed_up_key;

        char *tips;

    } hs_act_st;

    int hs_ani_init(hs_act_st *act, int disk_num, hs_struct_st *st, hs_map_st *map, hs_button_st *hs_btn);
    void hs_ani_deinit(hs_act_st *act);
    int hs_ani_reset(hs_act_st *act, int disk_num, hs_struct_st *st, hs_map_st *map, hs_button_st *hs_btn);

    void hs_ani_draw(hs_act_st *act, hs_struct_st *st);
    void hs_ani_update(hs_act_st *act, hs_struct_st *st, hs_map_st *map, hs_button_st *hs_btn);

    void hs_ani_stop(hs_act_st *act);
    void hs_ani_next(hs_act_st *act);
    void hs_ani_resume(hs_act_st *act);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HS_ANIMATION_H__ */
