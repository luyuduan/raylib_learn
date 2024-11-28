/**
 * Brief: 汉诺塔
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-18     lyd          Create
 *
 */

#ifndef __HANOI_H__
#define __HANOI_H__

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct record_st
    {
        int sour_column;
        int dest_column;
    } record_st;

    void hanoi(int n, record_st *record);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HANOI_H__ */
