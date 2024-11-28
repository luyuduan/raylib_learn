/**
 * Brief: 演示汉诺塔移动
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-18     lyd          Create
 *
 */

/* standard reference */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* public   reference */
#include "raylib.h"
#include "raymath.h"
#include "hanoi.h"
#include "hanoi_show.h"

/* private  reference */

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

#define COLUMN_NUM 3
#define INVALID_SIZE 0

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define FRAME_RATE 60

typedef int column_num_t;

typedef struct hanoi
{
    int disk_num;
    int disk_height;
    int disk_base_radius;

    int window_height;
    int window_width;
    int margin_horizontal; /* 左右，横向边距 */
    int margin_vertical;   /* 上下，纵向边距 */

    Vector2 base; /* 底座矩形的左上角 */

    float lineThick; /* 轮廓线的宽度 */

    Color disk_color;
    Color disk_str_color;
    int disk_str_font_size;

    Color pedestal_color;
    Vector2 pedestal_size;
    int space;          /* 底层圆盘间空隙宽 */
    int column_spacing; /* 柱子横向间距 */

    int column_extra; /* 柱子堆满圆盘后，柱子多出的高度，单位是圆盘高度 */
    int column_over;  /* 为了柱子间移动圆盘不贴着柱子顶，柱子上空多出一段，单位是圆盘高度 */
    Color column_color;
    Vector2 column_pos[COLUMN_NUM];
    Vector2 column_size;

    char *column_str[COLUMN_NUM];
    Color column_str_color;
    int column_str_font_size;
    int column_str_pos_y;

    char info_str[16];
    Vector2 info_str_pos;
    Color info_str_color;
    int info_str_font_size;

    column_num_t *map[COLUMN_NUM];

} hanoi_demo_st;

typedef enum move_step_en
{
    START = 0,
    UP,
    TURN_ONE,
    SHIFT,
    TURN_TWO,
    DOWN,
    END,
} move_step_en;

typedef struct column_disk_st
{
    int column;
    int disk;
    int x;
    int y;
} column_disk_st;

typedef struct hs_act_st
{
    int record_step; /* record的游标 */
    int record_len;

    column_disk_st sour;
    column_disk_st sour_top;
    column_disk_st dest_top;
    column_disk_st dest;
    int size;

    move_step_en move_state; /* 线段游标 */
    int shift;               /* 位移有正负 */
    int speed_list[FRAME_RATE];
    int speed_idx;  /* 线段位移，步进游标 */
    int speed_len;  /* 有效速度个数 */
    int shift_loop; /* 有效速度个数 */

    record_st *record;

} hs_act_st;

/*
    塔盘初始化
    基座、柱子、提示信息的布局信息


        []       []       []
        []       []       []
        []       []       []
    ############################
*/
static int hs_init(hanoi_demo_st *hanoi, int disk_num)
{
    int i = 0;

    memset(hanoi, 0x00, sizeof(hanoi_demo_st));
    hanoi->disk_num = disk_num;
    hanoi->disk_height = 20;
    hanoi->disk_base_radius = 20;

    hanoi->margin_vertical = 100;
    hanoi->margin_horizontal = 100;

    hanoi->space = 20;
    int widest_disk_width = hanoi->disk_num * hanoi->disk_base_radius * 2;
    /* 底座矩形，宽 = 4个留白 + 3个最宽圆盘占宽 */
    hanoi->pedestal_size = (Vector2){(COLUMN_NUM + 1) * hanoi->space +
                                         COLUMN_NUM * widest_disk_width,
                                     20};

    /* 柱子高一点，初始时，能看到柱子，3个圆盘高度 */
    hanoi->column_extra = 3;
    /* 柱子宽度可自定义，这里定义为圆盘的半径
        柱子高：所有圆盘占高 + 柱子高一段
    */
    hanoi->column_size = (Vector2){hanoi->disk_base_radius,
                                   (hanoi->disk_num + hanoi->column_extra) * hanoi->disk_height};

    /* 柱子上空留空，2个圆盘高度 */
    hanoi->column_over = 2;
    /* 顶上留白 = 上边界 + 柱子上空留空 */
    int top_margin = hanoi->margin_vertical + hanoi->column_over * hanoi->disk_height;
    /*
     * window构成
     * 高 = 顶上留白 + 柱子高 + 底座高 + 底部边距
     * 宽 = 两个边界 + 底座宽
     */
    hanoi->window_height = top_margin +
                           hanoi->column_size.y +
                           hanoi->pedestal_size.y +
                           hanoi->margin_vertical;

    hanoi->window_width = 2 * hanoi->margin_horizontal + hanoi->pedestal_size.x;

    /* 底座矩形的左上角 */
    hanoi->base = (Vector2){
        hanoi->margin_horizontal,
        hanoi->window_height - hanoi->margin_vertical - hanoi->pedestal_size.y};

    hanoi->lineThick = 2;

    hanoi->disk_color = WHITE;
    hanoi->disk_str_color = BLUE;
    hanoi->disk_str_font_size = 20;

    hanoi->pedestal_color = YELLOW;

    hanoi->column_color = YELLOW;
    hanoi->column_spacing = hanoi->space + widest_disk_width;
    int column_x_start = hanoi->base.x + hanoi->space +
                         hanoi->disk_num * hanoi->disk_base_radius - hanoi->column_size.x / 2;
    for (i = 0; i < COLUMN_NUM; i++)
    {
        hanoi->column_str[i] = (char *[]){"A", "B", "C"}[i];
        hanoi->column_pos[i] = (Vector2){column_x_start + i * hanoi->column_spacing,
                                         hanoi->base.y - hanoi->column_size.y};
    }
    hanoi->column_str_color = BLUE;
    hanoi->column_str_font_size = 20;
    hanoi->column_str_pos_y = hanoi->base.y + hanoi->pedestal_size.y;
    hanoi->info_str_color = BLUE;
    hanoi->info_str_font_size = 20;
    hanoi->info_str_pos = (Vector2){
        hanoi->base.x, hanoi->column_str_pos_y + hanoi->info_str_font_size * 2};

    column_num_t *map = calloc(COLUMN_NUM, hanoi->disk_num * sizeof(hanoi->map[0][0]));
    if (NULL == map)
    {
        printf("malloc failed\n");
        return -1;
    }

    for (i = 0; i < COLUMN_NUM; i++)
    {
        hanoi->map[i] = map + i * hanoi->disk_num;
    }

    for (i = 0; i < hanoi->disk_num; i++)
    {
        hanoi->map[0][i] = i + 1; /* size 从1开始 */
    }

    return 0;
}

static void hs_deinit(hanoi_demo_st *hanoi)
{
    if (hanoi->map[0])
    {
        free(hanoi->map[0]);
    }
}

/* 产生匀加速运动的位移量序列 */
static int gravity_style(int *p, int len, int max_x)
{
    int i = 0;
    int fix = 1;

    if (max_x < 0)
    {
        fix = -1;
    }

    /* 加速度 */
    int a = fix * 2;
    // int a = fix * max_x * 2 / len / len;

    /* 0时刻 */
    int v = 0;
    int x = 0; /* 统计位移 */
    for (i = 0; i < len; i++)
    {
        v += a;
        x += v;
        p[i] = v;
        if (x * fix > max_x * fix)
        {
            p[i] = max_x;
            return i;
        }
    }

    p[len - 1] = max_x;

    return len;
}

/* 生成 匀速运动的位移量 序列 */
static int constant_speed_style(int *p, int len, int max_x)
{
    int rate = max_x / len;
    for (int i = 0; i < len; i++)
    {
        p[i] = rate;
    }

    return len;
}

/*
 * 画出三根柱子、基座、提示信息
 */
static void paint_pedestal_column(hanoi_demo_st *hanoi)
{
    for (int i = 0; i < COLUMN_NUM; i++)
    {
        Rectangle rec = (Rectangle){hanoi->column_pos[i].x,
                                    hanoi->column_pos[i].y,
                                    hanoi->column_size.x,
                                    hanoi->column_size.y + 3}; /* +3表示柱子插入底座 */
        DrawRectangle(rec.x, rec.y, rec.width, rec.height, hanoi->column_color);
        DrawRectangleLinesEx(rec, hanoi->lineThick, BLACK);

        int str_len = MeasureText(hanoi->column_str[i], hanoi->disk_str_font_size);
        DrawText(hanoi->column_str[i],
                 rec.x + str_len / 2, hanoi->column_str_pos_y + 3, /* +3表示柱子标注下移少许 */
                 hanoi->column_str_font_size, hanoi->column_str_color);
    }

    Rectangle rec = (Rectangle){hanoi->base.x,
                                hanoi->base.y,
                                hanoi->pedestal_size.x,
                                hanoi->pedestal_size.y};
    DrawRectangleRec(rec, hanoi->pedestal_color);
    DrawRectangleLinesEx(rec, hanoi->lineThick, BLACK);

    DrawText(hanoi->info_str,
             hanoi->info_str_pos.x, hanoi->info_str_pos.y,
             hanoi->info_str_font_size, hanoi->info_str_color);
}

static int size_to_width(hanoi_demo_st *hanoi, int size)
{
    return size * hanoi->disk_base_radius * 2;
}

/*
    xy为圆盘矩形左上角坐标
 */
static void paint_disk_pos(hanoi_demo_st *hanoi, int x, int y, int size)
{
    int width = size_to_width(hanoi, size);

    Rectangle rec = (Rectangle){x, y, width, hanoi->disk_height};

    DrawRectangleRounded(rec, 1, 90, hanoi->disk_color);
    DrawRectangleRoundedLinesEx(rec, 1, 90, hanoi->lineThick, BLACK);

    char str[16];
    snprintf(str, sizeof(str), "%d", size);
    int str_len = MeasureText(str, hanoi->disk_str_font_size);
    DrawText(str, x + width / 2 - str_len / 2, y, hanoi->disk_str_font_size, hanoi->disk_str_color);
}

/*
 * 输入：圆盘位置x为[0,1,2]，y为[0,1,2,...,MAX_N]，表示从上往下数第几个
 * 输出：圆盘左上角坐标
 *  */
static void get_pos(hanoi_demo_st *hanoi, column_disk_st *pos, int size)
{
    int width = size_to_width(hanoi, size);

    pos->x = hanoi->column_pos[pos->column].x + hanoi->column_size.x / 2 - width / 2;
    pos->y = hanoi->base.y - (hanoi->disk_num - pos->disk) * hanoi->disk_height;
}

static void flush_map(hanoi_demo_st *hanoi)
{
    paint_pedestal_column(hanoi);

    for (int i = 0; i < COLUMN_NUM; i++)
    {
        for (int j = 0; j < hanoi->disk_num; j++)
        {
            int size = hanoi->map[i][j];
            if (INVALID_SIZE != size)
            {
                column_disk_st pos;
                pos.column = i;
                pos.disk = j;
                get_pos(hanoi, &pos, size);
                paint_disk_pos(hanoi, pos.x, pos.y, size);
            }
        }
    }
}

static void get_move_line_len(hanoi_demo_st *hanoi, hs_act_st *frame_cache)
{
    switch (frame_cache->move_state)
    {
    case START:
        frame_cache->shift =
            (frame_cache->sour_top.disk - frame_cache->sour.disk) * hanoi->disk_height;
        break;
    case TURN_ONE:
        frame_cache->shift =
            (frame_cache->dest_top.column - frame_cache->sour_top.column) * hanoi->column_spacing;
        break;
    case TURN_TWO:
        frame_cache->shift =
            (frame_cache->dest.disk - frame_cache->dest_top.disk) * hanoi->disk_height;
        break;
    default:
        break;
    }
}

/*
 * 功能：获取移动的圆盘的信息，sour最上面的圆盘，到，dest最上面
 */
static void read_record(hanoi_demo_st *hanoi, hs_act_st *frame_cache)
{
    record_st *record = &(frame_cache->record[frame_cache->record_step]);

    int sour_column = record->sour_column;
    int dest_column = record->dest_column;

    if (sour_column == dest_column)
    {
        return;
    }

    /*  移动前圆盘坐标  */
    frame_cache->sour.column = sour_column;
    frame_cache->sour.disk = 0;
    int *column = hanoi->map[sour_column];
    for (int i = 0; i < hanoi->disk_num; i++)
    {
        if (INVALID_SIZE != column[i])
        {
            frame_cache->sour.disk = i;
            frame_cache->size = column[i];
            column[i] = INVALID_SIZE;
            break;
        }
    }

    /* 目的坐标 */

    frame_cache->dest.column = dest_column;
    frame_cache->dest.disk = hanoi->disk_num - 1;
    column = hanoi->map[dest_column];
    for (int i = 0; i < hanoi->disk_num; i++)
    {
        if (INVALID_SIZE != column[i])
        {
            frame_cache->dest.disk = i - 1;
            break;
        }
    }

    frame_cache->sour_top.column = sour_column;
    frame_cache->sour_top.disk = -(hanoi->column_extra + hanoi->column_over);
    frame_cache->dest_top.column = dest_column;
    frame_cache->dest_top.disk = -(hanoi->column_extra + hanoi->column_over);
}

static int move_info_init(hs_act_st *frame_cache, int disk_num)
{
    memset(frame_cache, 0x00, sizeof(hs_act_st));
    frame_cache->record_step = 0;
    frame_cache->move_state = START;
    frame_cache->record_len = pow(2, disk_num) - 1;
    frame_cache->record = calloc(frame_cache->record_len, sizeof(record_st));
    if (NULL == frame_cache->record)
    {
        printf("malloc failed!\n");
        return -1;
    }
    hanoi(disk_num, frame_cache->record);

    return 0;
}

static void move_info_deinit(hs_act_st *frame_cache)
{
    if (frame_cache->record)
    {
        free(frame_cache->record);
    }
}

void draw_frame(hanoi_demo_st *hanoi, hs_act_st *frame_cache)
{
    hs_act_st *m = frame_cache;
    int x, y;

    flush_map(hanoi);

    /*
        (sour_column_idx, top_disk_idx)   -->  (dest_column_idx, top_disk_idx)
            ^                                           |
            |                                           |
            |                                           v
        (sour_column_idx, sour_disk_idx)       (dest_column_idx, dest_disk_idx)
    */
    if (START == m->move_state)
    {
        read_record(hanoi, m);
        snprintf(hanoi->info_str, sizeof(hanoi->info_str),
                 "%s --> %s  %d",
                 hanoi->column_str[m->sour.column], hanoi->column_str[m->dest.column], m->size);

        get_move_line_len(hanoi, m);
        m->speed_len = gravity_style(m->speed_list, ARRAY_LEN(m->speed_list),
                                     m->shift);

        m->move_state = UP;
        get_pos(hanoi, &m->sour, m->size);
        m->shift_loop = 0;
        m->speed_idx = 0;
    }

    if (UP == m->move_state)
    {
        m->shift_loop += m->speed_list[m->speed_len - 1 - m->speed_idx]; /* 匀减速 */
        x = m->sour.x;
        y = m->sour.y + m->shift_loop;
        paint_disk_pos(hanoi, x, y, m->size);

        m->speed_idx++;
        if (m->speed_idx >= m->speed_len)
        {
            m->move_state = TURN_ONE;

            return;
        }
    }

    if (TURN_ONE == m->move_state)
    {
        get_move_line_len(hanoi, m);
        m->speed_len = constant_speed_style(m->speed_list, ARRAY_LEN(m->speed_list),
                                            m->shift);
        m->move_state = SHIFT;
        get_pos(hanoi, &m->sour_top, m->size);
        m->shift_loop = 0;
        m->speed_idx = 0;
    }

    if (SHIFT == m->move_state)
    {
        m->shift_loop += m->speed_list[m->speed_idx];
        x = m->sour_top.x + m->shift_loop;
        y = m->sour_top.y;
        paint_disk_pos(hanoi, x, y, m->size);

        m->speed_idx++;
        if (m->speed_idx >= m->speed_len)
        {
            m->move_state = TURN_TWO;
            return;
        }
    }

    if (TURN_TWO == m->move_state)
    {
        get_move_line_len(hanoi, m);
        m->speed_len = gravity_style(m->speed_list, ARRAY_LEN(m->speed_list), m->shift);
        m->move_state = DOWN;
        get_pos(hanoi, &m->dest_top, m->size);
        m->shift_loop = 0;
        m->speed_idx = 0;
    }

    if (DOWN == m->move_state)
    {
        m->shift_loop += m->speed_list[m->speed_idx];
        x = m->dest_top.x;
        y = m->dest_top.y + m->shift_loop;
        paint_disk_pos(hanoi, x, y, m->size);

        m->speed_idx++;
        if (m->speed_idx >= m->speed_len)
        {
            m->move_state = END;
            return;
        }
    }

    if (END == m->move_state)
    {
        hanoi->map[m->dest.column][m->dest.disk] = m->size;

        flush_map(hanoi);
        m->record_step++;
        m->move_state = START;
    }
}

void hanoi_show(int disk_num)
{
    int ret = 0;
    hanoi_demo_st hanoi_demo;
    hs_act_st frame_cache;

    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hanoi Tower by lyd");
    SetTargetFPS(FRAME_RATE);

    ret = hs_init(&hanoi_demo, disk_num);
    assert(0 == ret);
    ret = move_info_init(&frame_cache, hanoi_demo.disk_num);
    assert(0 == ret);

    SetWindowSize(hanoi_demo.window_width, hanoi_demo.window_height);
    while (!WindowShouldClose())
    {
        // 绘制帧
        BeginDrawing();

        ClearBackground(WHITE);

        flush_map(&hanoi_demo);
        if (frame_cache.record_step < frame_cache.record_len)
        {
            draw_frame(&hanoi_demo, &frame_cache);
        }

        EndDrawing();
    }

    move_info_deinit(&frame_cache);
    hs_deinit(&hanoi_demo);

    CloseWindow();
}
