/**
 * Brief: 按钮控件
 *  ref: https://www.bilibili.com/video/BV13B4y1c7HX/?p=8
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
#include "button.h"

/* private  reference */

void button_init(button_st *btn, const char *text)
{
    memset(btn, 0x00, sizeof(button_st));

    btn->text = text;

    btn->bg_color = DARKGRAY;

    btn->normal_color = LIGHTGRAY;
    btn->select_color = WHITE;
    btn->font_color = btn->normal_color;
    btn->font_size = 20;

    btn->rec.width = 200;
    btn->rec.height = 50;
}

/**
 * @brief 设置坐标
 *
 * @param[in] btn -
 * @param[in] x - 矩形左边界 x
 * @param[in] y - 矩形上边界 y
 *
 * @note
 * @see
 */
void button_set_pos(button_st *btn, int x, int y)
{
    btn->rec.x = x;
    btn->rec.y = y;
}

/**
 * @brief 设置坐标
 *
 * @param[in] btn -
 * @param[in] x - 矩形中心 x
 * @param[in] y - 矩形中心 y
 *
 * @note
 * @see
 */
void button_set_pos_center(button_st *btn, int x, int y)
{
    btn->rec.x = x - btn->rec.width / 2;
    btn->rec.y = y - btn->rec.height / 2;
}

void button_set_callback(button_st *btn, button_cb_pf callback, void *callback_arg)
{
    btn->callback = callback;
    btn->callback_arg = callback_arg;
}

void button_del_callback(button_st *btn)
{
    btn->callback = NULL;
    btn->callback_arg = NULL;
}

void button_draw_in(button_st *btn, const char *text)
{
    DrawRectangleRec(btn->rec, btn->bg_color);
    DrawText(text,
             btn->rec.x + btn->rec.width / 2 - MeasureText(text, btn->font_size) / 2,
             btn->rec.y + btn->rec.height / 2 - btn->font_size / 2,
             btn->font_size, btn->font_color);
}

void button_draw(button_st *btn)
{
    button_draw_in(btn, btn->text);
}

bool button_update_in(button_st *btn, button_cb_pf callback, void *callback_arg)
{
    btn->font_color = btn->normal_color;
    if (CheckCollisionPointRec(GetMousePosition(), btn->rec))
    {
        btn->font_color = btn->select_color;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (callback)
            {
                callback(callback_arg);
            }
            return true;
        }
    }

    if (IsKeyPressed(btn->bind_key))
    {
        if (callback)
        {
            callback(callback_arg);
        }
        return true;
    }

    return false;
}

bool button_update(button_st *btn)
{
    return button_update_in(btn, btn->callback, btn->callback_arg);
}

void button_bind_key(button_st *btn, int key)
{
    btn->bind_key = key;
}

Vector2 button_get_size(button_st *btn)
{
    return (Vector2){btn->rec.width, btn->rec.height};
}

void button_set_size(button_st *btn, int width, int height)
{
    btn->rec.width = width;
    btn->rec.height = height;
}

void button2_init(button2_st *btn2, const char *text)
{
    button_init(BTN2_BTN(btn2), text);
    btn2->state = true; /* true，默认使用主体 */
}

void button2_attach(button2_st *btn2, const char *text, button_cb_pf callback, void *callback_arg)
{
    btn2->text = text;
    btn2->callback = callback;
    btn2->callback_arg = callback_arg;
}

void button2_reset(button2_st *btn2, const char *text)
{
    button2_init(btn2, text);
}

void button2_draw(button2_st *btn2)
{
    if (btn2->state)
    {
        button_draw(BTN2_BTN(btn2));
    }
    else
    {
        button_draw_in(BTN2_BTN(btn2), btn2->text);
    }
}

bool button2_update(button2_st *btn2)
{
    bool if_click = false;
    if (btn2->state)
    {
        if_click = button_update(BTN2_BTN(btn2));
    }
    else
    {
        if_click = button_update_in(BTN2_BTN(btn2), btn2->callback, btn2->callback_arg);
    }
    if (if_click)
    {
        btn2->state = !btn2->state;
    }

    return if_click;
}

void button2_set_state(button2_st *btn2, bool if_def_state)
{
    btn2->state = if_def_state;
}

void button2_del_callback(button2_st *btn2)
{
    button_del_callback(BTN2_BTN(btn2));
    btn2->callback = NULL;
    btn2->callback_arg = NULL;
}
