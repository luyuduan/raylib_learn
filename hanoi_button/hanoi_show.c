/**
 * Brief: 演示汉诺塔移动
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-18     lyd          Create
 *
 */

/* standard reference */
#include <assert.h>
#include <stdlib.h>
/* public   reference */
#include "dbg.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "hanoi.h"
#include "hanoi_show.h"
#include "hs_button.h"
#include "hs_anim.h"
#include "hs_structure.h"
#include "hs_disk_map.h"
#include "hs_tips.h"

/* private  reference */

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define FRAME_RATE 60

typedef struct hanoi_demo_st
{
    int disk_num;

    hs_struct_st st;

    hs_map_st map;

    hs_button_st button;

    hs_act_st act;

    hs_tips_st tips;

} hanoi_demo_st;

int hs_init(hanoi_demo_st *hd, int disk_num)
{
    int ret          = 0;
    hs_struct_st *st = &hd->st;

    hs_structure_init(st, disk_num);

    ret = hs_disk_map_init(&hd->map, disk_num);
    if (0 != ret)
    {
        DBG_ERR("map init failed\n");
        return -1;
    }

    hs_button_init(&hd->button, st, hd);

    ret = hs_ani_init(&hd->act, disk_num, &hd->st, &hd->map, &hd->button);
    if (0 != ret)
    {
        DBG_ERR("animation init failed\n");
        return -1;
    }

    hs_tip_init(&hd->tips);

    hd->disk_num = disk_num;

    SetWindowSize(hd->st.window_width, hd->st.window_height);

    return 0;
}

void hs_update(hanoi_demo_st *hd)
{
    hs_button_update(&hd->button);
    hs_ani_update(&hd->act, &hd->st, &hd->map, &hd->button);
}

void hs_draw(hanoi_demo_st *hd)
{
    hs_structure_draw(&hd->st);
    hs_button_draw(&hd->button);
    hs_disk_map_draw(&hd->map, &hd->st);
    hs_ani_draw(&hd->act, &hd->st);
    hs_tip_draw(&hd->tips, &hd->st, &hd->act, &hd->button);
}

void hs_deinit(hanoi_demo_st *hd)
{
    hs_ani_deinit(&hd->act);
    hs_disk_map_deinit(&hd->map);
}

int hs_reset(hanoi_demo_st *hd)
{
    int ret          = 0;
    hs_struct_st *st = &hd->st;

    int disk_num = hs_button_get_disk_num_set(&hd->button);

    hs_structure_reset(st, disk_num);

    ret = hs_disk_map_reset(&hd->map, disk_num);
    if (0 != ret)
    {
        DBG_ERR("map reset failed\n");
        return -1;
    }

    hs_button_reset(&hd->button, st, hd);

    ret = hs_ani_reset(&hd->act, disk_num, st, &hd->map, &hd->button);
    if (0 != ret)
    {
        DBG_ERR("animation reset failed\n");
        return -1;
    }

    hd->disk_num = disk_num;

    SetWindowSize(hd->st.window_width, hd->st.window_height);

    return 0;
}

void hanoi_show(int disk_num)
{
    int ret = 0;
    hanoi_demo_st hd;

    SetTraceLogLevel(LOG_NONE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hanoi Tower by lyd");
    SetTargetFPS(FRAME_RATE);

    ret = hs_init(&hd, disk_num);
    assert(0 == ret);
    while (!WindowShouldClose())
    {
        // 更新帧
        hs_update(&hd);

        // 绘制帧
        BeginDrawing();
        ClearBackground(WHITE);
        hs_draw(&hd);

        EndDrawing();
    }

    hs_deinit(&hd);

    CloseWindow();
}

void button_clicked_cb_next(void *data)
{
    hanoi_demo_st *hd = (hanoi_demo_st *)data;
    hs_ani_next(&hd->act);
}

void button_clicked_cb_reset(void *data)
{
    hanoi_demo_st *hd = (hanoi_demo_st *)data;
    int ret           = hs_reset(hd);
    if (0 != ret)
    {
        hs_deinit(hd);
        DBG_ERR("hs_reset failed! exit...\n");
        exit(-1);
    }
}

void button_clicked_cb_stop(void *data)
{
    hanoi_demo_st *hd = (hanoi_demo_st *)data;
    hs_ani_stop(&hd->act);
}

void button_clicked_cb_resume(void *data)
{
    hanoi_demo_st *hd = (hanoi_demo_st *)data;
    hs_ani_resume(&hd->act);
}
