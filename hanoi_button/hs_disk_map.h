/**
 * Brief: 汉诺塔演示 圆盘映射
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-21     lyd          Create
 *
 */

#ifndef __HS_MAP_H__
#define __HS_MAP_H__
#include "hs_structure.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef int column_num_t;

    typedef struct hs_map_st
    {
        column_num_t *map[COLUMN_NUM];
        int disk_num;
    } hs_map_st;

    int hs_disk_map_init(hs_map_st *hs_map, int disk_num);
    void hs_disk_map_deinit(hs_map_st *hs_map);
    int hs_disk_map_reset(hs_map_st *hs_map, int disk_num);
    void hs_disk_map_draw(hs_map_st *hs_map, hs_struct_st *st);

    bool hs_disk_map_size_if_valid(int size);
    int hs_disk_map_get_size(hs_map_st *hs_map, int column, int disk);
    int hs_disk_map_set_size(hs_map_st *hs_map, int column, int disk, int size);
    int hs_disk_map_get_top(hs_map_st *hs_map, int column, int *disk, int *size);
    int hs_disk_map_get_seat(hs_map_st *hs_map, int column, int *disk);
    void hs_disk_map_paint_disk(hs_struct_st *st, int x, int y, int size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HS_MAP_H__ */
