/**
 * Brief: 演示 animation 使用
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-18     lyd          Create
 *
 */

/* standard reference */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
/* public   reference */
#include "raylib.h"
#include "raymath.h"
#include "animation.h"

/* private  reference */

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 740
#define FRAME_RATE 60

#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

#define DEMO_NUM ANI_BUTT

typedef struct ani_demo_st
{
    ani_frame_st frame;
    char *name;
    Vector2 name_pos;
    char rate_str[32];
    Color ball_color;

} ani_demo_st;

typedef struct ani_demo_mng_st
{
    ani_demo_st style_demo[DEMO_NUM];

    int name_font_size;
    int rate_pos_x;

    int check;
    int wait;

} ani_demo_mng_st;

int radius = 10;
#define CHANGE (WINDOW_WIDTH / 2)
int centerX_start = (WINDOW_WIDTH - CHANGE) / 2;
int centerY_start = 20;

void demo_init(ani_demo_mng_st *d, int len, int start_x, int change)
{
    d->name_font_size = radius * 2;
    d->rate_pos_x = start_x + change + radius * 2;
    for (int i = 0; i < len; i++)
    {
        ani_demo_st *t = &d->style_demo[i];
        ani_frame_st *f = &t->frame;

        Vector2 start = {centerX_start, centerY_start + i * radius * 3};
        Vector2 end = {centerX_start + CHANGE, centerY_start + i * radius * 3};
        animation_init(f, start, end, (animation_type_en)i);
        animation_start(f);
        t->name = animation_style_name[i];
        t->name_pos = (Vector2){
            start_x - MeasureText(t->name, d->name_font_size) - radius * 2,
            centerY_start + i * radius * 3};
        t->ball_color = WHITE;
        memset(t->rate_str, 0x00, sizeof(t->rate_str));
    }
    d->check = 0;
    d->wait = 120;
}

bool demo_update(ani_demo_mng_st *d, int len)
{
    ani_demo_st *t = &d->style_demo[d->check];
    ani_frame_st *f = &t->frame;

    bool if_done = animation_update(f);

    if (if_done)
    {
        int x_end = centerX_start + CHANGE;
        Vector2 x_cache;
        x_cache = animation_x(f);
        if (FloatEquals(x_cache.x, x_end))
        {
            t->ball_color = GREEN;
        }
        else
        {
            snprintf(t->rate_str, ARRAY_LEN(t->rate_str), "%d/%d", (int)x_cache.x, x_end);
        }

        d->check++; /* 逐个演示 */
        if (d->check >= len)
        {
            d->check = len - 1;
            d->wait--;
            if (d->wait <= 0)
            {
                return true;
            }
        }
    }

    return false;
}

void demo_draw(ani_demo_mng_st *d, int len)
{
    /* 起始、终止线 */
    int x_s = centerX_start;
    int x_e = centerX_start + CHANGE;
    DrawLineEx((Vector2){x_s, 0}, (Vector2){x_s, WINDOW_HEIGHT}, 2, BLACK);
    DrawLineEx((Vector2){x_e, 0}, (Vector2){x_e, WINDOW_HEIGHT}, 2, BLACK);

    for (int i = 0; i < len; i++)
    {
        ani_demo_st *t = &d->style_demo[i];
        ani_frame_st *f = &t->frame;

        Vector2 x_cache;
        x_cache = animation_x(f);
        int y = t->name_pos.y;

        DrawText(t->name, t->name_pos.x, y, d->name_font_size, BLACK);
        DrawCircle(x_cache.x, x_cache.y, radius, t->ball_color);
        DrawText(t->rate_str, d->rate_pos_x, y, d->name_font_size, BLACK);
    }
}

void demo_restart(ani_demo_mng_st *d, int len, int start_x, int change)
{
    demo_init(d, len, start_x, change);
}

int main(void)
{
    ani_demo_mng_st demo;

    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "animation demo");
    SetTargetFPS(FRAME_RATE);

    demo_init(&demo, DEMO_NUM, centerX_start, CHANGE);

    while (!WindowShouldClose())
    {
        // 更新帧
        bool if_done = demo_update(&demo, DEMO_NUM);
        if (if_done)
        {
            demo_init(&demo, DEMO_NUM, centerX_start, CHANGE);

            /* 循环演示 */
        }

        // 绘制帧
        BeginDrawing();

        ClearBackground(GRAY);
        demo_draw(&demo, DEMO_NUM);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
