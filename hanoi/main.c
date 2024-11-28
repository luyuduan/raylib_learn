/**
 * Brief: 汉诺塔图示
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-16     lyd          Create
 *
 */

/* standard reference */
#include <stdio.h>

/* public   reference */
#include "hanoi.h"
#include "hanoi_show.h"
/* private  reference */

int main(void)
{
    int disk_num = 5;

    // hanoi(disk_num, NULL);

    hanoi_show(disk_num);

    return 0;
}
