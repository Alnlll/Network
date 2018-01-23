#include <stdio.h>
#include <stdarg.h> //va_xxx(),
#include <string.h> //memcpy()
#include "log.h"
#include "common.h"

char *log_colors[LOG_LEVEL_NUM] = {RED, L_RED, YELLOW, GREEN, DEFAULT_COLOR};
char *log_prefix_str[LOG_LEVEL_NUM] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG"};

int log_print(FILE *f, char *format, va_list args)
{
    int err = 0;
    
    if (NULL == f)
    {
        err = -1;
    }

    if (0 > (err = vfprintf(f, format, args)))
    {
        perror("log_print()-file");
    }

    return err;
}
int log_write(log *p_log, log_level ll, char *format, ...)
{
    int err = 0;
    char s_time[64] = {0};
    va_list args;

    if ((NULL == p_log) || \
       ((NULL == p_log->f) && (NULL == p_log->s)) || \
       (p_log->limit_lv < ll))
    {
        err = -1;
        goto err_end;
    }
    
    SET_COLOR(log_colors[ll]);

    get_str_time(s_time, sizeof(s_time), LOG_PREFIX_TIME_FORM, LOCAL_TIME);
    if (NULL != p_log->f)
    {
        fprintf(p_log->f, LOG_PREFIX_FORM, s_time, log_prefix_str[ll]);
        va_start(args, format);
        if (0 > (err = log_print(p_log->f, format, args)))
        {
            err = -1;
            goto err_end;
        }
        va_end(args);
    }
    if (NULL != p_log->s)
    {
        fprintf(p_log->s, LOG_PREFIX_FORM, s_time, log_prefix_str[ll]);
        va_start(args, format);
        if (0 > (err = log_print(p_log->s, format, args)))
        {
            err = -1;
            goto err_end;
        }
        va_end(args);
    }

    goto end;
    
err_end:
    va_end(args);
    SET_COLOR(DEFAULT_COLOR);
    return err;
end:
    SET_COLOR(DEFAULT_COLOR);
    return err;
}

