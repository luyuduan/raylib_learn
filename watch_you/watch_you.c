/**
 * Brief: 眼睛注视鼠标
 *      瞳孔表示瞳孔和虹膜
 *      raylib练手项目
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-14     lyd          Create
 *
 */

/* standard reference */
#include <raylib.h>
#include <raymath.h>

/* public   reference */

/* private  reference */

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 740
#define FRAME_RATE 60

typedef struct eye_st
{
    int eye_socket_radius_x; /* 眼眶半径 */
    int eye_socket_radius_y; /* 眼眶半径 */
    int eye_socket_radius;   /* 眼眶窄半径 */
    Vector2 pos;             /* 眼眶中心坐标 */

    int pupil_radius;     /* 瞳孔半径 */
    Vector2 pupil_pos;    /* 瞳孔坐标 */
    int radius_ratio;     /* 瞳孔到眼眶中心的半径 */
    int radius_ratio_max; /* 瞳孔到眼眶中心的最大半径 */
    int agility;          /* 灵敏度 */
    Texture buffer;       /* 缓存 */

    Color eye_socket; /* 眼眶颜色 */
    Color eye_ball;   /* 眼白颜色 */
    Color iris;       /* 虹膜颜色 */
    Color pupil;      /* 瞳孔颜色 */

} eye_st;

void my_clamp(int *value, int min, int max)
{
    int result = (*value < min) ? min : *value;

    if (result > max)
    {
        result = max;
    }

    *value = result;
}

int min(int a, int b)
{
    if (a > b)
    {
        return b;
    }
    return a;
}

int max(int a, int b)
{
    if (a < b)
    {
        return b;
    }
    return a;
}

void eye_init(eye_st *eye, int eye_socket_radius_x, int eye_socket_radius_y,
              int pupil_radius)
{
    eye->eye_socket_radius_x = eye_socket_radius_x;
    eye->eye_socket_radius_y = eye_socket_radius_y;
    eye->eye_socket_radius = min(eye->eye_socket_radius_x, eye->eye_socket_radius_y);
    eye->pos = Vector2Zero();

    eye->pupil_radius = pupil_radius;
    eye->pupil_pos = Vector2Zero();
    eye->radius_ratio = 0;
    eye->radius_ratio_max = eye->eye_socket_radius - eye->pupil_radius; /* 眼珠出去了？试着减少最大值ratio_max */
    eye->agility = max(2, eye->radius_ratio_max / FRAME_RATE);          /* 每帧率移动的像素 */

    eye->eye_socket = BLACK;
    eye->eye_ball = LIGHTGRAY;
    eye->iris = DARKGREEN;
    eye->pupil = BLACK;

    // 使用缓存
    Image pupil_img = GenImageColor(eye->eye_socket_radius_x * 2, eye->eye_socket_radius_y * 2, BLANK);
    ImageDrawCircle(&pupil_img, eye->pupil_radius, eye->pupil_radius, eye->pupil_radius, eye->iris);      /* 棕色虹膜 */
    ImageDrawCircle(&pupil_img, eye->pupil_radius, eye->pupil_radius, eye->pupil_radius / 2, eye->pupil); /* 黑色瞳孔 */
    eye->buffer = LoadTextureFromImage(pupil_img);
    UnloadImage(pupil_img); // img不再改动，故释放掉img
}

void eye_deinit(eye_st *eye)
{
    UnloadTexture(eye->buffer);
}

void eye_setPos(eye_st *eye, int x, int y)
{
    eye->pos.x = x;
    eye->pos.y = y;
    eye->pupil_pos = eye->pos;
}

void eye_horizontal_alignment(eye_st *eye, int num)
{
    /* 眼珠等高。那就取个均值吧 */
    int agv = 0;
    for (int i = 0; i < num; i++)
    {
        agv += eye[i].pupil_pos.y / num;
    }

    for (int i = 0; i < num; i++)
    {
        eye[i].pupil_pos.y = agv;
    }
}

void eye_draw(eye_st *eye)
{
    // 绘制静态的眼眶，10是眼眶厚度
    DrawEllipse(eye->pos.x, eye->pos.y, eye->eye_socket_radius_x + 10, eye->eye_socket_radius_y + 10, eye->eye_socket);
    DrawEllipse(eye->pos.x, eye->pos.y, eye->eye_socket_radius_x, eye->eye_socket_radius_y, eye->eye_ball);
    // DrawCircleV(eye->pos, 4, BLACK); // 眼眶中心 辅助理解

    // 绘制动态的瞳孔
    DrawTextureV(eye->buffer,
                 Vector2SubtractValue(eye->pupil_pos, eye->pupil_radius),
                 WHITE);
}

void eye_WatchMouse(eye_st *eye, Vector2 mouse_pos)
{
    if (CheckCollisionPointCircle(mouse_pos, eye->pos, eye->radius_ratio_max))
    {
        /* 眼眶内部，直接盯住 */
        eye->pupil_pos = mouse_pos;
    }
    else
    {
        /* 相似三角形 */
        /* 改变相似比模拟眼珠移动 */
        if (!IsCursorOnScreen())
        {
            /* 回正 */
            eye->radius_ratio -= eye->agility;
        }
        else
        {
            /* 注视、跟随 */
            eye->radius_ratio += eye->agility;
        }
        my_clamp(&(eye->radius_ratio), 0, eye->radius_ratio_max);

        float pos_mouse_distance = Vector2Distance(eye->pos, mouse_pos);
        /*  斜边比rat = radius_ratio / pos_mouse_distance
                pupil_x +
                        |
             center --- * ----- x
                   | \
                   |  \
         pupil_y - *    @
                   |     \
                   |      \
                   y       @ mouse
            同时也是直角边的比rat = (pupil_pos - pos) / (mouse_pos - pos)
            pupil_pos = pos + rat * (mouse_pos - pos)
        */
        eye->pupil_pos = Vector2Lerp(eye->pos, mouse_pos, eye->radius_ratio / pos_mouse_distance);
    }
}

int main(void)
{
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Watching you");

    /* 眼睛的属性 */
    int eye_num = 2;               /* 眼睛个数 */
    int eye_socket_radius_x = 100; /* 眼眶椭圆水平半径 */
    int eye_socket_radius_y = 100; /* 眼眶椭圆垂直半径 */
    my_clamp(&eye_socket_radius_x, 20, WINDOW_WIDTH);
    my_clamp(&eye_socket_radius_y, 20, WINDOW_HEIGHT);

    /* 瞳孔半径。别超出椭圆就行，这里取较小半径的一半 */
    int pupil_radius = min(eye_socket_radius_x, eye_socket_radius_y) / 2;

    /* 布局。居中对齐  */
    int pupil_distance = eye_socket_radius_x * 3; /* 瞳距，水平间距 */
    int eye_y = WINDOW_HEIGHT / 4;                /* 眼睛y坐标 */

    my_clamp(&eye_num, 1, WINDOW_WIDTH / pupil_distance);

    int eye_start_pos_x = (WINDOW_WIDTH - ((eye_num - 1) * pupil_distance)) / 2;

    eye_st eyes[eye_num]; /* 就这么分配 */

    for (int i = 0; i < eye_num; i++)
    {
        eye_init(&eyes[i], eye_socket_radius_x, eye_socket_radius_y, pupil_radius);
        eye_setPos(&eyes[i], eye_start_pos_x + i * pupil_distance, eye_y);
    }

    SetTargetFPS(FRAME_RATE);
    while (!WindowShouldClose())
    {
        // 更新帧
        Vector2 mouse_pos = GetMousePosition();
        for (int i = 0; i < eye_num; i++)
        {
            eye_WatchMouse(&eyes[i], mouse_pos);
        }

        eye_horizontal_alignment(eyes, eye_num);

        // 绘制帧
        BeginDrawing();

        ClearBackground(DARKGRAY);
        // ClearBackground(BLANK); /* 透明背景，可能不生效 */

        for (int i = 0; i < eye_num; i++)
        {
            eye_draw(&eyes[i]);
        }

        if (IsCursorOnScreen())
        {
            for (int i = 0; i < eye_num; i++)
            {
                // DrawLineEx(eyes[i].pos, mouse_pos, 1.0, RED); // 射线 辅助理解
            }
        }

        EndDrawing();
    }

    // 释放texture对象
    for (int i = 0; i < eye_num; i++)
    {
        eye_deinit(&eyes[i]);
    }

    CloseWindow();

    return 0;
}

/*

参考：
https://github.com/raysan5/raylib/blob/master/examples/shapes/shapes_following_eyes.c
https://www.bilibili.com/video/BV1zNCXYaED
https://github.com/tstamborski/wineyes
https://jj811208.github.io/watching-you/
https://github.com/kmecke/WpfEyes

更多：
    无窗口运行
    椭圆眼眶
    瞳孔中心到眼眶边缘

 */
