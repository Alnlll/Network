#ifndef _LOG_H_
#define _LOG_H_

#define LOG_DIR "./log"
#define CMD_MK_LOG_DIR "mkdir ./log"
#define LOG_PREFIX_TIME_FORM "%Y-%m-%d_%X"
#define LOG_PREFIX_FORM "%s %s: "

typedef enum
{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_NUM
} log_level;

typedef struct _log
{
    FILE *f; /* logfile */
    FILE *s; /* stdout or stderr */
    log_level limit_lv; /* Only the log with equal or lower value will be outputed. */
    //char *color_conf[LOG_LEVEL_NUM];
} log;

int log_print(FILE *f, char *format, va_list args);
int log_process(log *p_log, log_level ll, char *format, ...);
#if 0
int log_fatal(FILE *f, char *format, ...);
int log_error(FILE *f, char *format, ...);
int log_warn(FILE *f, char *format, ...);
int log_info(FILE *f, char *format, ...);
int log_debug(FILE *f, char *format, ...);
#endif //0
#endif //_LOG_H_