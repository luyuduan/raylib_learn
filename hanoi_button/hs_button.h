/**
 * Brief: 汉诺塔演示 按钮组件控制
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-20     lyd          Create
 *
 */

#ifndef __HS_BUTTON_H__
#define __HS_BUTTON_H__
#include "button.h"
#include "hs_structure.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct hs_button_st
    {
        button_st btn[HS_BTN_BUTT];
        button2_st btn2[HS_BTN2_BUTT];

        int button_width;
        int button_height;

        char *tips;

        int disk_num;
        int disk_num_set; /* 输入框指定的圆盘数 */
        bool edit_mode;
        int disk_num_set_min;
        int disk_num_set_max;

    } hs_button_st;

    void hs_button_init(hs_button_st *hs_btn, hs_struct_st *st, void *demo);
    void hs_button_update(hs_button_st *hs_btn);
    void hs_button_draw(hs_button_st *hs_btn);
    void hs_button_reset(hs_button_st *hs_btn, hs_struct_st *st, void *demo);
    void hs_button_btn2_switch_def(hs_button_st *hs_btn);
    void hs_button_btn2_switch_attach(hs_button_st *hs_btn);
    void hs_button_del_callback(hs_button_st *hs_btn);
    int hs_button_get_disk_num_set(hs_button_st *hs_btn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HS_BUTTON_H__ */
