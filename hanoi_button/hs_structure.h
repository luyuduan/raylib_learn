/**
 * Brief: 汉诺塔演示 塔体布局结构
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-20     lyd          Create
 *
 */

#ifndef __HS_STRUCTURE_H__
#define __HS_STRUCTURE_H__
#include "raylib.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define COLUMN_NUM 3

    typedef enum hs_button_en
    {
        HS_BTN_RESET = 0,
        HS_BTN_NEXT,
        HS_BTN_BUTT,
    } hs_button_en;

    typedef enum hs_button2_en
    {
        HS_BTN2_STOP_RESUME = 0,
        HS_BTN2_BUTT,

    } hs_button2_en;

    /* 圆盘定位 */
    typedef struct column_disk_st
    {
        int column; /* 横向，左数柱子数。0，1，2 */
        int disk;   /* 纵向，从上往下数圆盘个数，初始堆叠的顶层索引为0 */

        /*
        disk 可以为负数
        表示圆盘在纵向上能移动的范围，以圆盘厚度/高度为单位

            disk
            -5          已脱离柱子的范围，高度由 column_over 描述，图示中 column_over = 2
            -4          该脱离范围用于圆盘在柱子间移动

            -3    #     柱子高出圆盘的范围，高度由 column_extra 描述
            -2    #     图示中即 column_extra = 3
            -1    #     柱子高出一部分，稳妥++，美观++

            0    (#)    映射在 map 数组中，map[column][disk] = size
            1   ( # )
            2  (  #  )
            #####################
                  0              column
         */

        int x; /* 绝对坐标 */
        int y; /* 绝对坐标 */
    } column_disk_st;

    /**
     * 汉诺塔画面布局
     *
     *  底座、石柱、提示信息、按钮的布局信息
     *
     *      []       []       []
     *      []       []       []
     *      []       []       []
     *  ############################
     *      A        B        C
     *  info                  button
     *
     * */
    typedef struct hs_struct_st
    {
        /* 窗口、留白布局 */
        int window_height;
        int window_width;
        int margin_horizontal; /* 左右，横向边距 */
        int margin_up;         /* 上，边距 */
        int margin_down;       /* 上下，纵向边距 */
        float lineThick;       /* 轮廓线的宽度 */

        /* 圆盘布局 */
        int disk_num;
        int disk_height;
        int disk_base_radius;
        Color disk_color;
        Color disk_str_color;
        int disk_str_font_size;

        /* 底座布局 */
        Vector2 base; /* 底座矩形的左上角 */
        Color pedestal_color;
        Vector2 pedestal_size;

        /* 石柱布局 */
        int space;          /* 底层圆盘间空隙宽 */
        int column_spacing; /* 柱子横向间距 */
        int column_extra;   /* 柱子堆满圆盘后，柱子多出的高度，单位是圆盘高度 */
        int column_over;    /* 为了柱子间移动圆盘不贴着柱子顶，柱子上空多出一段，单位是圆盘高度 */
        Color column_color;
        Vector2 column_pos[COLUMN_NUM];
        Vector2 column_size;
        /* 石柱标记布局 */
        char *column_str[COLUMN_NUM];
        Color column_str_color;
        int column_str_font_size;
        int column_str_pos_y;

        /* 步骤信息布局 */
        Vector2 info_str_pos;
        Color info_str_color;
        int info_str_font_size;

        /* 按钮布局 */
        int button_width;
        int button_height;
        int button_num;
        int button_x;
        int button_y;

        /* 提示信息布局 */
        Vector2 tips_pos;
        Color tip_str_color;
        int tip_str_font_size;
        Vector2 tips_check_box_pos;

        /* 指定圆盘范围 */
        int disk_num_set_min;
        int disk_num_set_max;

    } hs_struct_st;

    void hs_structure_init(hs_struct_st *st, int disk_num);
    void hs_structure_draw(hs_struct_st *st);
    void hs_structure_reset(hs_struct_st *st, int disk_num);

    void hs_structure_get_pos(hs_struct_st *st, column_disk_st *pos, int size);
    int hs_structure_size_to_width(hs_struct_st *st, int size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HS_STRUCTURE_H__ */
