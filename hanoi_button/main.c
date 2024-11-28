/**
 * Brief: 汉诺塔图示
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-16     lyd          Create
 *
 */

/* standard reference */

/* public   reference */
#include "dbg.h"
#include "hanoi.h"
#include "hanoi_show.h"
/* private  reference */

char *dbg_module_prefix = "hanoi";
DBG_L_TYPE dbg_module_level = DBG_L_DBG;

int main(void)
{
    int disk_num = 5;

    // hanoi(disk_num, NULL);

    hanoi_show(disk_num);

    return 0;
}
