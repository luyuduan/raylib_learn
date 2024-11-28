/**
 * Brief: 按钮控件
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-18     lyd          Create
 *
 */

#ifndef __BUTTON_H__
#define __BUTTON_H__
#include "raylib.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef void (*button_cb_pf)(void *);

    typedef struct button_st
    {
        Rectangle rec;
        const char *text;
        Color font_color;
        unsigned int font_size;
        Color bg_color;

        Color normal_color; /* 可以用于背景色或字体色 */
        Color select_color;

        button_cb_pf callback;
        void *callback_arg;

        int bind_key;
    } button_st;

    typedef struct button2_st
    {
        button_st _btn;

        bool state;

        const char *text;
        button_cb_pf callback;
        void *callback_arg;

    } button2_st;
    typedef enum button2_switch_en
    {
        BTN2_SWITCH_DEFAULT,
        BTN2_SWITCH_ATTACH,
        BTN2_SWITCH_BUTT,
    } button2_switch_en;

    void button_init(button_st *btn, const char *text);
    void button_set_pos(button_st *btn, int x, int y);
    void button_set_pos_center(button_st *btn, int x, int y);
    void button_set_callback(button_st *btn, button_cb_pf callback, void *callback_arg);
    void button_del_callback(button_st *btn);
    Vector2 button_get_size(button_st *btn);
    void button_set_size(button_st *btn, int width, int height);
    void button_draw(button_st *btn);
    bool button_update_in(button_st *btn, button_cb_pf callback, void *callback_arg);
    bool button_update(button_st *btn);
    void button_bind_key(button_st *btn, int key);

    void button2_init(button2_st *btn2, const char *text);
    void button2_attach(button2_st *btn2, const char *text, button_cb_pf callback, void *callback_arg);
    void button2_reset(button2_st *btn2, const char *text);
    void button2_draw(button2_st *btn2);
    bool button2_update(button2_st *btn2);
    void button2_set_state(button2_st *btn2, bool if_def_state);
    void button2_del_callback(button2_st *btn2);

#define BTN2_BTN(btn2) (&(btn2)->_btn)
#define button2_set_pos(btn2, x, y) button_set_pos(BTN2_BTN(btn2), x, y)
#define button2_set_pos_center(btn2, x, y) button_set_pos_center(BTN2_BTN(btn2), x, y)
#define button2_set_callback(btn2, cb, d) button_set_callback(BTN2_BTN(btn2), cb, d)
#define button2_get_size(btn2) button_get_size(BTN2_BTN(btn2))
#define button2_set_size(btn2, w, h) button_set_size(BTN2_BTN(btn2), w, h)
#define button2_bind_key(btn2, key) button_bind_key(BTN2_BTN(btn2), key)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BUTTON_H__ */
