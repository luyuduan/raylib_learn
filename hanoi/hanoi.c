/**
 * Brief: 汉诺塔
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
#include "hanoi.h"

/* private  reference */

static void print(int from, int to)
{
    printf("%c --> %c\n", from + 'A', to + 'A');
}

static void hanoi_print(int n, int from, int to, int buffer)
{
    if (1 == n)
    {
        print(from, to);
    }
    else
    {
        /* 【移动】之前，先把n-1块圆盘从 源柱 搬到 缓存柱，目标柱用做缓存  */
        hanoi_print(n - 1, from, buffer, to);

        /* 此时n-1块圆盘 已经 从源柱搬到缓存柱了，于是【移动】  */
        print(from, to); /* 写成本行，少一次函数调用，也可写成下行 */
        // hanoi_print(1, from, to, buffer);

        /* 【移动】之后，把剩余的n-1块圆盘，从缓存柱 搬到 目标柱，源柱用做缓存 */
        hanoi_print(n - 1, buffer, to, from);
    }
}

static void record(int from, int to, record_st *rd)
{
    static size_t cnt = 0;

    rd[cnt].sour_column = from;
    rd[cnt].dest_column = to;
    cnt++;
}

static void hanoi_record(int n, int from, int to, int buffer, record_st *rd)
{
    if (1 == n)
    {
        record(from, to, rd);
    }
    else
    {
        /* 【移动】之前，先把n-1块圆盘从 源柱 搬到 缓存柱，目标柱用做缓存柱  */
        hanoi_record(n - 1, from, buffer, to, rd);

        /* 此时n-1块圆盘 已经 从源柱搬到缓存柱了，于是【移动】  */
        record(from, to, rd); /* 写成本行，少一次函数调用，也可写成下行 */
        // hanoi_record(1, from, to, buffer, rd);

        /* 【移动】之后，把剩余的n-1块圆盘，从缓存柱 搬到 目标柱  */
        hanoi_record(n - 1, buffer, to, from, rd);
    }
}

/**
 * @brief 汉诺塔打印或记录过程
 *
 * @param[in] disk_num - 圆盘数
 * @param[in out] rd - 缓存数组，记录每次移动，长度至少为pow(2,n)-1
 *
 * @note
 * @see
 */
void hanoi(int disk_num, record_st *rd)
{
    if (rd)
    {
        hanoi_record(disk_num, 0, 2, 1, rd);
    }
    else
    {
        hanoi_print(disk_num, 0, 2, 1);
    }
}

/*
 * 汉诺塔（Tower of Hanoi），又称河内塔，是一个源于印度古老传说的益智玩具。
 * 大梵天创造世界的时候做了三根金刚石柱子，在一根柱子上从下往上按照大小顺序摞着64片黄金圆盘。
 * 大梵天命令婆罗门把圆盘从下面开始按大小顺序重新摆放在另一根柱子上。
 * 并且规定，在小圆盘上不能放大圆盘，在三根柱子之间一次只能移动一个圆盘。
 *
 * 分A B C三柱，初始N块圆盘都在A柱上。
 * 【移动】：欲将A柱最底下的1块圆盘移到C最底下，必须A柱只有一块圆盘，B柱有N-1块圆盘，C柱无圆盘。然后A->C。
 * 【移动】之前，要解决B柱上N-1块圆盘怎么来的，
 * 这就相当于把N-1块圆盘从A柱(原起始柱作起始柱)移动到B柱(原缓存柱作目标柱)上，这里递归调用自己。
 * 【移动】之后，要解决B柱上N-1块圆盘移动到C柱，
 * 这就相当于把N-1块圆盘从B柱(原缓存柱作起始柱)移动到C柱(原目标柱作目标柱)上，这里也递归调用自己。
 * A柱或B柱没有圆盘时搬完。
 * 一言以蔽之，永远是在搬运两块圆盘——N块和[N-1]块。
 * 所以分析后，ABC柱的角色或者作用不是确定的，分起始柱、目标柱和缓存柱。移动是确定的，都是起始柱->目标柱。
 *
 * 步数与圆盘个数的关系：
 * 上述【移动】后，那最大的一块(第n块)就不再移动了。
 * 而移动它，“总共”需要 Y[n] = 2 * Y[n-1]+ 1步，当n=1时，一块圆盘，总共只需移动一次，故Y[1] = 1。
 *
 * 上述是递推式，下面求解通项公式：化上式为
 *
 *      Y[n]   + 1 = 2 * (Y[n-1] + 1)，
 *
 * 则有
 *
 *      Y[n-1] + 1 = 2 * (Y[n-2] + 1),
 *      ...,
 *      Y[2]   + 1 = 2 * (Y[1]   + 1),
 *
 * 等式两边分别相乘，有
 *
 *      (Y[n] + 1]) * (Y[n-1] + 1]) *...* (Y[2] + 1]
 *    = 2^(n-1)     * (Y[n-1] + 1]) * (Y[n-2] + 1]) *...* (Y[1] + 1])
 *
 * 等号两边同除非0同项，得到
 *
 *      Y[n] + 1 = 2^(n-1) * (Y[1] + 1)
 *
 * 也可以把化式中 “Y[n]+1” 和 “Y[n-1]+1” 视为整体，套用等比数列通项公式，
 *
 *      A[n] = A[1] * q^(n-1)，其中q是公比。
 *
 * 对应到当前场景，q等于2，于是得到
 *
 *      Y[n] + 1 = (Y[1] + 1) * 2^(n-1)
 *
 *  因为Y[1] = 1，上式 Y[n] = 2^n - 1
 *
 * 结论：n块圆盘，至少需要移动2^n - 1次才能完成移动
 *
 * 按照大梵天的命令，n=64，婆罗门至少需要搬运圆盘2^64 - 1次才能完成任务。
 * 这和“棋盘麦粒问题”中一样数量麦粒
 *
 */
