/**
 * Brief: 汉诺塔演示 圆盘映射
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-21     lyd          Create
 *
 */

/* standard reference */
#include <stdlib.h>

/* public   reference */
#include "dbg.h"
#include "hs_disk_map.h"

/* private  reference */
#define INVALID_SIZE 0

void map_load(hs_map_st *hs_map, int disk_num)
{
    hs_map->disk_num = disk_num;

    for (int i = 0; i < hs_map->disk_num; i++)
    {
        hs_map->map[0][i] = i + 1; /* size 从1开始 */
    }

    for (int i = 1; i < COLUMN_NUM; i++)
    {
        for (int j = 0; j < hs_map->disk_num; j++)
        {
            hs_map->map[i][j] = INVALID_SIZE; /* 填充无效值 */
        }
    }
}

int hs_disk_map_init(hs_map_st *hs_map, int disk_num)
{
    column_num_t *map = calloc(COLUMN_NUM, disk_num * sizeof(column_num_t));
    if (NULL == map)
    {
        DBG_ERR("malloc failed\n");
        return -1;
    }

    for (int i = 0; i < COLUMN_NUM; i++)
    {
        hs_map->map[i] = map + i * disk_num;
    }

    map_load(hs_map, disk_num);

    return 0;
}

void hs_disk_map_deinit(hs_map_st *hs_map)
{
    if (hs_map->map[0])
    {
        free(hs_map->map[0]);
        for (int i = 0; i < COLUMN_NUM; i++)
        {
            hs_map->map[i] = NULL;
        }
    }
}

int hs_disk_map_get_size(hs_map_st *hs_map, int column, int disk)
{
    if ((column >= COLUMN_NUM) || (column < 0))
    {
        DBG_ERR("Error: column %d, not in [%d, %d]\n", column, 0, COLUMN_NUM);
        return -1;
    }
    if ((disk >= hs_map->disk_num) || (disk < 0))
    {
        DBG_ERR("Error: disk %d, not in [%d, %d]\n", disk, 0, hs_map->disk_num);
        return -1;
    }

    return hs_map->map[column][disk];
}

bool hs_disk_map_size_if_valid(int size)
{
    return (size != INVALID_SIZE);
}

int hs_disk_map_reset(hs_map_st *hs_map, int disk_num)
{
    hs_disk_map_deinit(hs_map);
    return hs_disk_map_init(hs_map, disk_num);
}

int hs_disk_map_set_size(hs_map_st *hs_map, int column, int disk, int size)
{
    int size_in = hs_disk_map_get_size(hs_map, column, disk);
    if (hs_disk_map_size_if_valid(size_in))
    {
        DBG_ERR("Error pos (%d, %d), the target position already has a disc",
                column, disk);
        return -1;
    }

    hs_map->map[column][disk] = size;

    return 0;
}

int pop_disk(hs_map_st *hs_map, int column, int disk)
{
    int size_in = hs_disk_map_get_size(hs_map, column, disk);
    if (!hs_disk_map_size_if_valid(size_in))
    {
        DBG_ERR("Error pos (%d, %d), there is no disc at the target location\n",
                column, disk);
        return -1;
    }

    hs_map->map[column][disk] = INVALID_SIZE;
    return 0;
}

int hs_disk_map_get_top(hs_map_st *hs_map, int column, int *disk, int *size)
{
    if (column >= COLUMN_NUM)
    {
        DBG_ERR("error column %d>=%d\n", column, COLUMN_NUM);
        return -1;
    }

    column_num_t *column_list = hs_map->map[column];

    for (int i = 0; i < hs_map->disk_num; i++)
    {
        if (hs_disk_map_size_if_valid(column_list[i]))
        {
            *disk = i;
            *size = column_list[i];
            pop_disk(hs_map, column, i);
            return 0;
        }
    }

    DBG_ERR("Error: There is no disc at the column %d\n", column);

    return -1;
}

/**
 * @brief 获取指定柱子上的空座
 *
 * @param[in] hs_map -
 * @param[in] column -
 * @param[in] disk -
 * @return int
 *
 * @note
 * @see
 */
int hs_disk_map_get_seat(hs_map_st *hs_map, int column, int *disk)
{
    if (column >= COLUMN_NUM)
    {
        DBG_ERR("error column %d>=%d\n", column, COLUMN_NUM);
        return -1;
    }

    column_num_t *column_list = hs_map->map[column];

    int disk_temp = hs_map->disk_num - 1;
    for (int i = 0; i < hs_map->disk_num; i++)
    {
        if (hs_disk_map_size_if_valid(column_list[i]))
        {
            disk_temp = i - 1;
            break;
        }
    }

    if (disk_temp < 0)
    {
        DBG_ERR("Error: No empty seat at the column %d\n", column);
        return -1;
    }

    *disk = disk_temp;
    return 0;
}

void hs_disk_map_paint_disk(hs_struct_st *st, int x, int y, int size)
{
    int width = hs_structure_size_to_width(st, size);

    Rectangle rec = (Rectangle){x, y, width, st->disk_height};

    DrawRectangleRounded(rec, 1, 90, st->disk_color);
    DrawRectangleRoundedLinesEx(rec, 1, 90, (int)st->lineThick, BLACK);

    char str[16];
    snprintf(str, sizeof(str), "%d", size);
    int str_len = MeasureText(str, st->disk_str_font_size);
    DrawText(str, x + width / 2 - str_len / 2, y, st->disk_str_font_size,
             st->disk_str_color);
}

void hs_disk_map_paint_disk_cd(hs_struct_st *st, int column, int disk, int size)
{
    column_disk_st pos;
    pos.column = column;
    pos.disk   = disk;
    hs_structure_get_pos(st, &pos, size);
    hs_disk_map_paint_disk(st, pos.x, pos.y, size);
}

void hs_disk_map_draw(hs_map_st *hs_map, hs_struct_st *st)
{
    for (int i = 0; i < COLUMN_NUM; i++)
    {
        for (int j = 0; j < st->disk_num; j++)
        {
            int size = hs_disk_map_get_size(hs_map, i, j);
            if (hs_disk_map_size_if_valid(size))
            {
                hs_disk_map_paint_disk_cd(st, i, j, size);
            }
        }
    }
}
