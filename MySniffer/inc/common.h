#ifndef _COMMON_H_
#define _COMMON_H_

#include <time.h> //time(), ...

#define DEFAULT_COLOR        "\e[0m"
#define BLACK                "\e[0;30m"
#define L_BLACK              "\e[1;30m"
#define RED                  "\e[0;31m"
#define L_RED                "\e[1;31m"
#define GREEN                "\e[0;32m"
#define L_GREEN              "\e[1;32m"
#define BROWN                "\e[0;33m"
#define YELLOW               "\e[1;33m"
#define BLUE                 "\e[0;34m"
#define L_BLUE               "\e[1;34m"
#define PURPLE               "\e[0;35m"
#define L_PURPLE             "\e[1;35m"
#define CYAN                 "\e[0;36m"
#define L_CYAN               "\e[1;36m"
#define GRAY                 "\e[0;37m"
#define WHITE                "\e[1;37m"
#define BOLD                 "\e[1m"
#define UNDERLINE            "\e[4m"
#define BLINK                "\e[5m"
#define REVERSE              "\e[7m"
#define HIDE                 "\e[8m"
#define CLEAR                "\e[2J"
#define CLRLINE              "\r\e[K" //or "\e[1K\r"
#define SET_COLOR(color)     printf("%s", (color))

#define LINE_DIS_NUM         16 //Display LINE_DIS_NUM data in hex in one line.

#define GET_DATADRIV_RES(index, key_val, res, table, key_name, proc_name) \
{\
    for (index = 0; index < sizeof(table)/sizeof(table[0]); index++)\
    {\
        if (key_val == table[index].key_name)\
        {\
            res = table[index].proc_name;\
            break;\
        }\
    }\
}

#if 0
/* Bi-direction queue implemention. */
#define QUEUE_ENTRY(type)\
struct\
{\
    type *next;\
    type *prev;\
}

#define QUEUE_HEAD(name, type)\
struct name\
{\
    type *q_first;\
    type *q_last;\
}

#define QUEUE_FIRST(head) ((head)->q_first)
#define QUEUE_LAST(head) ((head->q_last))
#define QUEUE_EMPTY(head) do {\
    QUEUE_FIRST(head) = NULL;\
    QUEUE_LAST(head) = NULL;\
}
#define QUEUE_ISEMPTY(head) (NULL == (head)->q_first)
#define QUEUE_NEXT(ele) ((ele)->next)
#define QUEUE_PREV(ele) ((ele)->prev)

#define QUEUE_INIT(head) do {\
    (head)->q_first = NULL;\
    (head)->q_last = NULL;\
} while(0)

#define QUEUE_IN(head, ele, field) do {\
    if (QUEUE_ISEMPTY(head))\
    {\
        QUEUE_FIRST(head) = ele;\
        QUEUE_LAST(head) = ele;\
        (ele)->next = NULL;\
        (ele)->prev = NULL;\
    }\
    else\
    {\
        QUEUE_LAST(head)->field.next = ele;\
        ele->field.prev = QUEUE_LAST(head);\
        ele->field.next = NULL;\
        head->last = ele;\
    }\
} while(0);

#define QUEUE_OUT(head, field, para) do {\
    if (QUEUE_ISEMPTY(head))\
        para = NULL;\
    else\
    {\
        para = QUEUE_FIRST(head);\
        (head)->q_first = QUEUE_FIRST(head)->next;\
        QUEUE_FIRST(head)->prev = NULL;\
    }\
} while(0);

#define QUEUE_FOREACH(head, field, index) \
for ((index) = QUEUE_FIRST(head);\
    (NULL != (index)) && ((index) != QUEUE_LAST(head));\
    (index) = (index)->(field).next)
#endif //0

/* Bi-deriction list definations */

#define BLIST_ENTRY(type)\
struct\
{\
    type *prev;\
    type *next;\
}

#define BLIST_HEAD(name, type)\
struct name\
{\
    type *first;\
}

/* Bi-deriction elements access methods */
#define BLIST_FIRST(head) ((head)->first)
#define BLIST_PREV(elm, field) ((elm)->field.prev)
#define BLIST_NEXT(elm, field) ((elm)->field.next)

/* Bi-deriction list functions */
#define BLIST_ISEMPTY(head) (NULL == (head)->first)

#define BLIST_INIT(head) do\
{\
    (head)->first = NULL;\
} while(0)

//Parameter elm should be NULL when add first member to list.
#define BLIST_INSERT_BEFORE(head, inelm, elm, field) do\
{\
    if (BLIST_ISEMPTY(head))\
    {\
        (head)->first = inelm;\
        (inelm)->field.prev = NULL;\
        (inelm)->field.next = NULL;\
    }\
    else\
    {\
        if (elm == BLIST_FIRST(head))\
            (head)->first = inelm;\
        if (NULL != (elm)->field.prev)\
            BLIST_PREV(elm, field)->field.next = inelm;\
        else\
            (head)->first = (inelm);\
        (inelm)->field.prev = (elm)->field.prev;\
        (inelm)->field.next = (elm);\
        (elm)->field.prev = (inelm);\
    }\
} while(0)

//Parameter elm should be NULL when add first member to list.
#define BLIST_INSERT_AFTER(head, inelm, elm, field) do\
{\
    if (BLIST_ISEMPTY(head))\
    {\
        (head)->first = inelm;\
        (inelm)->field.prev = NULL;\
        (inelm)->field.next = NULL;\
    }\
    else\
    {\
        if (NULL != BLIST_NEXT(elm, field))\
        {\
            BLIST_NEXT(elm, field)->field.prev = inelm;\
            (inelm)->field.next = BLIST_NEXT(elm, field);\
        }\
        else\
            (inelm)->field.next = NULL;\
        (inelm)->field.prev = (elm);\
        (elm)->field.next = (inelm);\
    }\
} while(0)

#define BLIST_REMOVE(head, elm, field) do\
{\
    if ((elm) == (head)->first)\
        (head)->first = (elm)->field.next;\
    if (NULL != BLIST_NEXT(elm, field))\
        BLIST_NEXT(elm, field)->field.prev = BLIST_PREV(elm, field);\
    if (NULL != BLIST_PREV(elm, field))\
        BLIST_PREV(elm, field)->field.next = BLIST_NEXT(elm, field);\
} while(0)

#define BLIST_FOREACH_HEAD(index, head, field) \
for (index = BLIST_FIRST(head);\
     index;\
     index = index->field.next)

/* Select type of time getted. */
typedef enum
{
    LOCAL_TIME = 0,
    UTC_TIME,
} time_type;

void hex_dump(char *data, int len);

void *zelloc(size_t size);
void free_mem(void *p);

/*
struct tm
{
    int tm_sec;
    int tm_min;
    int tm_hour; //since midnight - [0, 23]
    int tm_mday; //day of the month - [1, 31]
    int tm_mon; //since January - [0, 11]
    int tm_year; //since 1900
    int tm_wday; //since Sunday - [0, 6]
    int tm_yday; //since January 1 - [0, 365]
    int tm_isdst;
}
*/
struct tm *get_localtime(time_t *t);
struct tm *get_utctime(time_t *t);
/* time_2_str() formats:
*%a    缩写的星期几名称
*%A    完整的星期几名称
*%b    缩写的月份名称
*%B    完整的月份名称
*%c    日期和时间表示法	Sun Aug 19 02:56:02 2012
*%d    一月中的第几天（01-31）
*%H    24 小时格式的小时（00-23）
*%I    12 小时格式的小时（01-12）
*%j    一年中的第几天（001-366）
*%m    十进制数表示的月份（01-12）
*%M    分（00-59）
*%p    AM 或 PM
*%S    秒（00-61）
*%U    一年中的第几周，以第一个星期日作为第一周的第一天（00-53）
*%w    十进制数表示的星期几，星期日表示为 0（0-6）
*%W    一年中的第几周，以第一个星期一作为第一周的第一天（00-53）
*%x    日期表示法:08/19/12
*%X    时间表示法:02:50:06
*%y    年份，最后两个数字（00-99）
*%Y    年份
*%Z    时区的名称或缩写
*%%    一个 % 符号
*/
int time_2_str(char *str, int max_size, const char *format, const struct tm *p_tm);
int get_str_time(char *str, int max_size, const char *format, time_type tt);

FILE *open_file(char *path, char *mode);
void close_file(FILE *p);
int dir_exist(const char *name);

int exec_command_1(char *cmd);

#define DEBUG
#ifdef DEBUG
#define d_printf printf
#else
#define d_printf
#endif //DEBUG

#define LITTEL_ENDIAN //Byte order.

#endif //_COMMON_H_
