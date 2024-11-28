/**
 * Brief: 汉诺塔演示 提示信息
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-24     lyd          Create
 *
 */

/* standard reference */
#include <stdio.h>
#include <stdint.h>

/* public   reference */
#include "raygui.h"
#include "hs_anim.h"
#include "hs_button.h"
#include "hs_tips.h"

/* private  reference */

void hs_tip_init(hs_tips_st *tips)
{
    tips->if_open_tips = false;
}

void hs_tip_draw(hs_tips_st *tips, hs_struct_st *st, hs_act_st *act, hs_button_st *hs_btn)
{
    GuiCheckBox((Rectangle){st->tips_check_box_pos.x, st->tips_check_box_pos.y, 20, 20}, "Tips", &tips->if_open_tips);

    if (tips->if_open_tips)
    {
        DrawText(hs_btn->tips, st->tips_pos.x, st->tips_pos.y, st->tip_str_font_size, st->tip_str_color);
        DrawText(act->tips, st->tips_pos.x, st->tips_pos.y + st->tip_str_font_size, st->tip_str_font_size, st->tip_str_color);
    }
}
