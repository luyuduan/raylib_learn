/**
 * Brief: 汉诺塔演示 动画组件
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-20     lyd          Create
 *
 */

/* standard reference */
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
/* public   reference */
#include "dbg.h"
#include "hs_anim.h"

/* private  reference */

void track_stop(hs_act_st *act)
{
    animation_stop(&act->track);
}

void track_resume(hs_act_st *act)
{
    animation_resume(&act->track);
}

/**
 * @brief 一次直线为一个track
 *
 * @param[in/out] act -
 * @return true
 * @return false
 *
 * @note 默认停止
 * @see
 */
bool track_init(hs_act_st *act)
{
    column_disk_st *t = act->track_point[act->track_idx];
    Vector2 start = (Vector2){t->x, t->y};

    t = act->track_point[act->track_idx + 1];
    Vector2 end = (Vector2){t->x, t->y};

    animation_type_en style = act->line_style[act->track_idx];

    animation_init(&act->track, start, end, style);
    animation_set_speed_up_key(&act->track, act->speed_up_key);

    return false;
}

bool track_update(hs_act_st *act)
{
    bool if_done = animation_update(&act->track);
    if (if_done)
    {
        act->track_idx++;
        if (act->track_idx >= TRACK_NUM)
        {
            act->track_idx = TRACK_NUM - 1;
            return true;
        }
        track_init(act);
        track_resume(act);
    }

    return false;
}

/*
 * 功能：读取并解析一次解法
 */
void read_record(hs_act_st *act, hs_struct_st *st, hs_map_st *map)
{
    if (act->record_step >= act->record_len)
    {
        return;
    }
    record_st *record = &(act->record[act->record_step]);

    int sour_column = record->sour_column;
    int dest_column = record->dest_column;

    if (sour_column == dest_column)
    {
        return;
    }

    /*  得到路径轨迹  */
    act->sour.column = sour_column;
    hs_disk_map_get_top(map, act->sour.column, &act->sour.disk, &act->size);

    act->dest.column = dest_column;
    hs_disk_map_get_seat(map, act->dest.column, &act->dest.disk);

    act->sour_top.column = sour_column;
    act->sour_top.disk = -(st->column_extra + st->column_over);
    act->dest_top.column = dest_column;
    act->dest_top.disk = -(st->column_extra + st->column_over);
}

/* 一个record,一次action */
void act_init(hs_act_st *act, hs_struct_st *st, hs_map_st *map)
{
    read_record(act, st, map);
    snprintf(act->info_str, sizeof(act->info_str),
             "%d/%d  %s --> %s  %d", act->record_step, act->record_len,
             st->column_str[act->sour.column], st->column_str[act->dest.column], act->size);

    hs_structure_get_pos(st, &act->sour, act->size);
    hs_structure_get_pos(st, &act->sour_top, act->size);
    hs_structure_get_pos(st, &act->dest_top, act->size);
    hs_structure_get_pos(st, &act->dest, act->size);

    act->track_idx = 0;
}

bool act_update(hs_act_st *act, hs_struct_st *st, hs_map_st *map)
{
    bool if_done = track_update(act);

    if (if_done)
    {
        hs_disk_map_set_size(map, act->dest.column, act->dest.disk, act->size);
        act->record_step++;

        if (act->record_step >= act->record_len)
        {
            return true;
        }
        act_init(act, st, map);
        track_init(act);
        hs_ani_resume(act);

        if (act->if_run_once_act)
        {
            act->if_run_once_act = false;
            hs_ani_stop(act);
            hs_button_btn2_switch_attach(act->btn);
        }
    }

    return false;
}

void act_next(hs_act_st *act)
{
    bool if_stop = animation_if_stop(&act->track);
    if (if_stop)
    {
        act->if_run_once_act = true;
        track_resume(act);
        hs_button_btn2_switch_def(act->btn);
    }
}

int hs_ani_init(hs_act_st *act, int disk_num, hs_struct_st *st, hs_map_st *map, hs_button_st *hs_btn)
{
    int record_len = pow(2, disk_num) - 1;
    record_st *record = calloc(record_len, sizeof(record_st));
    if (NULL == record)
    {
        DBG_ERR("malloc failed!\n");
        return -1;
    }
    hanoi(disk_num, record);

    act->disk_num = disk_num;
    act->record_len = record_len;
    act->record = record;
    act->record_step = 0;
    act->info_str[0] = '\0';

    act->line_style[0] = ANI_OutSine;
    act->line_style[1] = ANI_CONST_V;
    act->line_style[2] = ANI_OutBounce;

    act->track_point[0] = &(act->sour);
    act->track_point[1] = &(act->sour_top);
    act->track_point[2] = &(act->dest_top);
    act->track_point[3] = &(act->dest);

    act->if_run_once_act = false;

    act->btn = hs_btn;

    act->speed_up_key = KEY_ENTER;
    act->tips = "Enter - speed up";

    act_init(act, st, map);
    track_init(act);

    return 0;
}

void hs_ani_update(hs_act_st *act, hs_struct_st *st, hs_map_st *map, hs_button_st *hs_btn)
{
    bool if_done = act_update(act, st, map);
    if (if_done)
    {
        hs_ani_stop(act);
        hs_button_del_callback(hs_btn);
    }
}

void hs_ani_draw(hs_act_st *act, hs_struct_st *st)
{
    Vector2 pos = animation_x(&act->track);
    hs_disk_map_paint_disk(st, pos.x, pos.y, act->size);

    /* 提示信息 */
    DrawText(act->info_str,
             st->info_str_pos.x, st->info_str_pos.y,
             st->info_str_font_size, st->info_str_color);
}

void hs_ani_deinit(hs_act_st *act)
{
    if (act->record)
    {
        free(act->record);
        act->record = NULL;
    }
}

/**
 * @brief 在停下状态按下，将到下个act结束。兼具步出功能
 *
 * @param[in/out] act -
 *
 * @note
 * @see
 */
void hs_ani_next(hs_act_st *act)
{
    act_next(act);
}

/**
 * @brief 帧级别
 *
 * @param[in/out] act -
 *
 * @note
 * @see
 */
void hs_ani_stop(hs_act_st *act)
{
    track_stop(act);
}

/**
 * @brief 帧级别
 *
 * @param[in/out] act -
 *
 * @note
 * @see
 */
void hs_ani_resume(hs_act_st *act)
{
    track_resume(act);
}

int hs_ani_reset(hs_act_st *act, int disk_num, hs_struct_st *st, hs_map_st *map, hs_button_st *hs_btn)
{
    hs_ani_deinit(act);
    return hs_ani_init(act, disk_num, st, map, hs_btn);
}
