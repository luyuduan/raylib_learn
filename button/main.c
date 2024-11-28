/**
 * Brief: 演示button使用
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-18     lyd          Create
 *
 */

/* standard reference */
#include <stdio.h>
#include <stdint.h>

/* public   reference */
#include "raylib.h"
#include "button.h"

/* private  reference */

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 740
#define FRAME_RATE 60

Color bgColor = DARKBLUE;

int is_same_color(Color a, Color b)
{
    if (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a)
    {
        return 1;
    }
    return 0;
}

void button_clicked_cb(void *args)
{
    (void)args;

    if (is_same_color(bgColor, BLACK))
    {
        bgColor = DARKBLUE;
    }
    else
    {
        bgColor = BLACK;
    }
}

int main(void)
{
    button_st btn;

    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "button demo");
    SetTargetFPS(FRAME_RATE);

    button_init(&btn, "change background color");
    button_set_callback(&btn, button_clicked_cb, NULL);
    button_set_pos_center(&btn, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    while (!WindowShouldClose())
    {
        // 更新帧
        button_update(&btn);

        // 绘制帧
        BeginDrawing();

        ClearBackground(bgColor);
        button_draw(&btn);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
