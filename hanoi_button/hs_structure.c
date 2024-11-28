/**
 * Brief: 汉诺塔演示 塔体布局结构
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-20     lyd          Create
 *
 */

/* standard reference */

/* public   reference */
#include "hs_structure.h"

/* private  reference */

/* 画面布局 */
void hs_structure_init(hs_struct_st *st, int disk_num)
{
    int i = 0;

    st->disk_num = disk_num;
    st->disk_height = 20;
    st->disk_base_radius = 20;

    st->margin_up = 20;
    st->margin_down = 150;
    st->margin_horizontal = 100;

    st->space = 20;
    int widest_disk_width = st->disk_num * st->disk_base_radius * 2;
    /* 底座矩形，宽 = 4个留白 + 3个最宽圆盘占宽 */
    st->pedestal_size = (Vector2){(COLUMN_NUM + 1) * st->space + COLUMN_NUM * widest_disk_width,
                                  20};

    /* 柱子高一点，初始时，能看到柱子，3个圆盘高度 */
    st->column_extra = 3;
    /* 柱子宽度可自定义，这里定义为圆盘的半径
        柱子高：所有圆盘占高 + 柱子高一段
    */
    st->column_size = (Vector2){st->disk_base_radius,
                                (st->disk_num + st->column_extra) * st->disk_height};

    /* 柱子上空留空，2个圆盘高度 */
    st->column_over = 2;
    /* 顶上留白 = 上边界 + 柱子上空留空 */
    int top_margin = st->margin_up + st->column_over * st->disk_height;
    /*
     * window构成
     * 高 = 顶上留白 + 柱子高 + 底座高 + 底部边距
     * 宽 = 两个边界 + 底座宽
     */
    st->window_height = top_margin + st->column_size.y + st->pedestal_size.y +
                        st->margin_down;

    st->window_width = 2 * st->margin_horizontal + st->pedestal_size.x;

    /* 底座矩形的左上角 */
    st->base = (Vector2){
        st->margin_horizontal,
        st->window_height - st->margin_down - st->pedestal_size.y};

    st->lineThick = 2;

    st->disk_color = WHITE;
    st->disk_str_color = BLUE;
    st->disk_str_font_size = 20;

    st->pedestal_color = YELLOW;

    st->column_color = YELLOW;
    st->column_spacing = st->space + widest_disk_width;
    int column_x_start = st->base.x + st->space +
                         st->disk_num * st->disk_base_radius - st->column_size.x / 2;
    for (i = 0; i < COLUMN_NUM; i++)
    {
        st->column_str[i] = (char *[]){"A", "B", "C"}[i];
        st->column_pos[i] = (Vector2){column_x_start + i * st->column_spacing,
                                      st->base.y - st->column_size.y};
    }
    st->column_str_color = BLUE;
    st->column_str_font_size = 20;
    st->column_str_pos_y = st->base.y + st->pedestal_size.y;
    st->info_str_color = BLUE;
    st->info_str_font_size = 20;
    st->info_str_pos = (Vector2){
        st->base.x, st->column_str_pos_y + st->column_str_font_size + st->info_str_font_size};

    st->button_width = 100;
    st->button_height = 25;
    st->button_num = (HS_BTN_BUTT + HS_BTN2_BUTT);
    st->button_x = st->base.x + st->pedestal_size.x - (st->button_num * st->button_width);
    st->button_y = st->info_str_pos.y + st->info_str_font_size;

    st->tips_pos = (Vector2){
        st->base.x,
        st->button_y + st->button_height,
    };
    st->tip_str_color = GRAY;
    st->tip_str_font_size = 20;
    st->tips_check_box_pos = (Vector2){st->base.x + st->pedestal_size.x, st->margin_up};

    st->disk_num_set_min = 3;
    st->disk_num_set_max = 10;
}

/*
 * 输入：圆盘位置x为[0,1,2]，y为[0,1,2,...,MAX_N]，表示从上往下数第几个
 * 输出：圆盘左上角坐标
 *  */
void hs_structure_get_pos(hs_struct_st *st, column_disk_st *pos, int size)
{
    int width = hs_structure_size_to_width(st, size);

    pos->x = st->column_pos[pos->column].x + st->column_size.x / 2 - width / 2;
    pos->y = st->base.y - (st->disk_num - pos->disk) * st->disk_height;
}

void hs_structure_reset(hs_struct_st *st, int disk_num)
{
    hs_structure_init(st, disk_num);
}

int hs_structure_size_to_width(hs_struct_st *st, int size)
{
    return size * st->disk_base_radius * 2;
}

/*
 * 画出三根柱子、基座
 */
void hs_structure_draw(hs_struct_st *st)
{
    /* 三根柱子 */
    for (int i = 0; i < COLUMN_NUM; i++)
    {
        Rectangle rec = (Rectangle){st->column_pos[i].x,
                                    st->column_pos[i].y,
                                    st->column_size.x,
                                    st->column_size.y + 3}; /* +3表示柱子插入底座 */
        DrawRectangle(rec.x, rec.y, rec.width, rec.height, st->column_color);
        DrawRectangleLinesEx(rec, st->lineThick, BLACK);

        int str_len = MeasureText(st->column_str[i], st->disk_str_font_size);
        DrawText(st->column_str[i],
                 rec.x + str_len / 2, st->column_str_pos_y + 3, /* +3表示柱子标注下移少许 */
                 st->column_str_font_size, st->column_str_color);
    }

    /* 底座 */
    Rectangle rec = (Rectangle){st->base.x,
                                st->base.y,
                                st->pedestal_size.x,
                                st->pedestal_size.y};
    DrawRectangleRec(rec, st->pedestal_color);
    DrawRectangleLinesEx(rec, st->lineThick, BLACK);
}
