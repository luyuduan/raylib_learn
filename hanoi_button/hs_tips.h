/**
 * Brief: 汉诺塔演示 提示信息
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-24     lyd          Create
 *
 */

#ifndef __HS_TIPS_H__
#define __HS_TIPS_H__

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct hs_tips_st
    {
        bool if_open_tips;

    } hs_tips_st;

    void hs_tip_init(hs_tips_st *tips);
    void hs_tip_draw(hs_tips_st *tips, hs_struct_st *st, hs_act_st *act, hs_button_st *hs_btn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HS_TIPS_H__ */
