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
int log_process(log *p_log, log_level ll, char *format, ...)
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

#if 0
int log_fatal(FILE *f, char *format, ...)
{
    int err = 0;
    char s_time[64] = {0};
    va_list args;

    if (NULL == f)
    {
        err = -1;
        goto end;
    }
    
    SET_COLOR(log_colors[LOG_LEVEL_FATAL]);

    get_str_time(s_time, sizeof(s_time), LOG_PREFIX_TIME_FORM, LOCAL_TIME);
    fprintf(f, LOG_PREFIX_FORM, s_time, "[FATAL]");

    va_start(args, format);
    if (0 > (err = log_print(f, format, args)))
    {
        err = -1;
        goto end;
    }
end:
    va_end(args);
    SET_COLOR(DEFAULT_COLOR);
    return err;
}

int log_error(FILE *f, char *format, ...)
{
    int err = 0;
    char s_time[64] = {0};
    va_list args;

    if (NULL == f)
    {
        err = -1;
        goto end;
    }
    
    SET_COLOR(log_colors[LOG_LEVEL_ERROR]);

    get_str_time(s_time, sizeof(s_time), LOG_PREFIX_TIME_FORM, LOCAL_TIME);
    fprintf(f, LOG_PREFIX_FORM, s_time, "[ERROR]");

    va_start(args, format);
    if (0 > (err = log_print(f, format, args)))
    {
        err = -1;
        goto end;
    }
end:
    va_end(args);
    SET_COLOR(DEFAULT_COLOR);
    return err;
}

int log_warn(FILE *f, char *format, ...)
{
    int err = 0;
    char s_time[64] = {0};
    va_list args;

    if (NULL == f)
    {
        err = -1;
        goto end;
    }
    
    SET_COLOR(log_colors[LOG_LEVEL_WARN]);
    
    get_str_time(s_time, sizeof(s_time), LOG_PREFIX_TIME_FORM, LOCAL_TIME);
    fprintf(f, LOG_PREFIX_FORM, s_time, "[WARN]");
    
    va_start(args, format);
    if (0 > (err = log_print(f, format, args)))
    {
        err = -1;
        goto end;
    }
end:
    va_end(args);
    SET_COLOR(DEFAULT_COLOR);
    return err;
}

int log_info(FILE *f, char *format, ...)
{
    int err = 0;
    char s_time[64] = {0};
    va_list args;

    if (NULL == f)
    {
        err = -1;
        goto end;
    }
    
    SET_COLOR(log_colors[LOG_LEVEL_INFO]);
    
    get_str_time(s_time, sizeof(s_time), LOG_PREFIX_TIME_FORM, LOCAL_TIME);
    fprintf(f, LOG_PREFIX_FORM, s_time, "[INFO]");
    
    va_start(args, format);
    if (0 > (err = log_print(f, format, args)))
    {
        err = -1;
        goto end;
    }
end:
    va_end(args);
    SET_COLOR(DEFAULT_COLOR);
    return err;
}

int log_debug(FILE *f, char *format, ...)
{
    int err = 0;
    char s_time[64] = {0};
    va_list args;

    if (NULL == f)
    {
        err = -1;
        goto end;
    }
    
    SET_COLOR(log_colors[LOG_LEVEL_DEBUG]);
    
    get_str_time(s_time, sizeof(s_time), LOG_PREFIX_TIME_FORM, LOCAL_TIME);
    fprintf(f, LOG_PREFIX_FORM, s_time, "[DEBUG]");
    
    va_start(args, format);
    if (0 > (err = log_print(f, format, args)))
    {
        err = -1;
        goto end;
    }
end:
    va_end(args);
    SET_COLOR(DEFAULT_COLOR);
    return err;
}
#endif //0

