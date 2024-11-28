/*
 *  dbg.h
 *
 *  编写时间: 2021/7/23
 *  编写者: wmh
 *
 *  功能描述：打印到标准输出，5级打印调试，分级颜色打印，包含打印定位，可多模块同时调用，只给**打印内容**颜色输出，**头部信息**不输出颜色
 *
 *  应用场景：所有
 *
 *  如果需要输出到文件，参见log版
 *
 *
 */
#ifndef _DBG_H_
#define _DBG_H_

#include <stdio.h>

/*
模块打印等级
要求：
1. 等级值越大，打印越多。
2. 默认打印最全。
3. 默认值为0
*/
typedef enum
{
    DBG_L_OFF = -5, /* 无打印输出 */
    DBG_L_COR,      /* 增加 核心信息，必须打印 */
    DBG_L_ERR,      /* 再增加 严重错误，软件模块无法自行恢复 */
    DBG_L_WRN,      /* 再增加 警告信息，已采取恢复措施的错误情况 */
    DBG_L_INF,      /* 再增加 情报信息，描述正常事件流的关键节点信息 */
    DBG_L_DBG = 0,  /* 再增加 调试信息，正常使用不需要的额外信息，打印最全 */
} DBG_L_TYPE;

/* 颜色信息 */
#ifdef DBG_COLOR
/* 前景色 */
#define DBG_COLOR_BLACK "30"   /* 黑色 */
#define DBG_COLOR_RED "31"     /* 红色 */
#define DBG_COLOR_GREEN "32"   /* 绿色 */
#define DBG_COLOR_YELLOW "33"  /* 黄色 */
#define DBG_COLOR_BLUE "34"    /* 蓝色 */
#define DBG_COLOR_CARMINE "35" /* 洋红 */
#define DBG_COLOR_CYAN "36"    /* 青色 */
#define DBG_COLOR_WHITE "37"   /* 白色 */

/* 显示方式 */
#define DBG_DEFAULT(COLOR) "\033[0;" COLOR "m"   /* 默认显示 */
#define DBG_HIGHLIGHT(COLOR) "\033[1;" COLOR "m" /* 高亮加粗 */
#define DBG_REVERSE(COLOR) "\033[7;" COLOR "m"   /* 反显 */
#define DBG_ITALIC(COLOR) "\033[3;" COLOR "m"    /* 斜体 */

/* 定义使用的颜色 */
#define DBG_RESET_COLOR DBG_DEFAULT()               /* 默认颜色 */
#define DBG_FILE_COLOR DBG_ITALIC(DBG_COLOR_WHITE)  /* 文件名   - 斜体 */
#define DBG_NOC_COLOR DBG_DEFAULT()                 /* 核心信息 - 默认颜色 */
#define DBG_ERR_COLOR DBG_DEFAULT(DBG_COLOR_RED)    /* 严重错误 - 红色 */
#define DBG_WRN_COLOR DBG_DEFAULT(DBG_COLOR_YELLOW) /* 警告信息 - 高亮黄色 */
#define DBG_INF_COLOR DBG_DEFAULT(DBG_COLOR_GREEN)  /* 情报信息 - 绿色 */
#define DBG_DBG_COLOR DBG_DEFAULT(DBG_COLOR_CYAN)   /* 调试信息 - 洋红 */

#else // DBG_COLOR
#define DBG_RESET_COLOR ""
#define DBG_FILE_COLOR ""
#define DBG_NOC_COLOR ""
#define DBG_ERR_COLOR ""
#define DBG_WRN_COLOR ""
#define DBG_INF_COLOR ""
#define DBG_DBG_COLOR ""

#endif // DBG_COLOR

/*模块打印前缀，打印信息中用以区分模块*/
extern char *dbg_module_prefix;

/*模块打印的最高等级
  建议：   开发时= DBG_L_DBG ，发行运行时=DBG_L_INF
*/
extern DBG_L_TYPE dbg_module_level;

/*
    头部信息                            打印内容
    DBG类型：模块名：文件(所在行):函数 打印内容
    例：
    ERROR:test0 test0.c(17):main{} 这是test0的打印
*/
#define DBG(dbg_level, ...)                \
    do                                     \
    {                                      \
        if (dbg_module_level >= dbg_level) \
        {                                  \
            printf(__VA_ARGS__);           \
        }                                  \
    } while (0)

/*
    makefile中使用宏__FILENAME__定义文件名，该宏不包含路径
 */
#ifndef __FILENAME__
#define __FILENAME__ __FILE__
#endif
/*
    头部信息                            打印内容
    DBG类型：模块名：文件(所在行):函数 打印内容
    例：
    ERROR:test0 test0.c(17):main{} 这是test0的打印
*/
#define IN_DBG(level, level_tag, color, fmt, ...)                \
    DBG(level, "%s%s %s %s%s:%d%s %s()%s " fmt "%s\r\n",         \
        DBG_RESET_COLOR, level_tag,                              \
        dbg_module_prefix,                                       \
        DBG_FILE_COLOR, __FILENAME__, __LINE__, DBG_RESET_COLOR, \
        __FUNCTION__, color, ##__VA_ARGS__, DBG_RESET_COLOR)

#ifdef DBG_ON

#define DBG_NOC(fmt, ...) IN_DBG(DBG_L_COR, "CORE ", DBG_NOC_COLOR, fmt, ##__VA_ARGS__)
#define DBG_ERR(fmt, ...) IN_DBG(DBG_L_ERR, "ERROR", DBG_ERR_COLOR, fmt, ##__VA_ARGS__)
#define DBG_WRN(fmt, ...) IN_DBG(DBG_L_WRN, "WARN ", DBG_WRN_COLOR, fmt, ##__VA_ARGS__)
#define DBG_INF(fmt, ...) IN_DBG(DBG_L_INF, "INFO ", DBG_INF_COLOR, fmt, ##__VA_ARGS__)
#define DBG_DBG(fmt, ...) IN_DBG(DBG_L_DBG, "DEBUG", DBG_DBG_COLOR, fmt, ##__VA_ARGS__)

#else

#define DBG_NOC(fmt, ...)
#define DBG_ERR(fmt, ...)
#define DBG_WRN(fmt, ...)
#define DBG_INF(fmt, ...)
#define DBG_DBG(fmt, ...)

#endif

#endif /* _DBG_H_ */
