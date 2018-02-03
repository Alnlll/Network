#include <stdio.h>
#include <stdlib.h> //malloc(), free(), system()
#include <string.h> //memset(),
#include <sys/types.h> //opendir(),
#include <dirent.h> //opendir(),
#include "common.h"

void hex_dump(char *data, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        if (!(i % LINE_DIS_NUM))
            printf("\n");
        printf("0x%02x ", *(data+i) & 0xff);
    }

    printf("\n");
}

void *zelloc(size_t size)
{
    void *p = NULL;

    if (NULL == (p = malloc(size)))
        return NULL;

    memset(p, 0, size);

    return p;
}

void free_mem(void *p)
{
    if (NULL != p)
    {
        free(p);
        p = NULL;
    }
}

int get_day_time(timeval_m *tm)
{
    int err = 0;
    struct timeval tv;
    struct timezone tz;

    if (NULL == tm)
    {
        err = -1;
        return err;
    }
    
    if (0 != (err = gettimeofday(&tv, &tz)))
    {
        perror("get_day_time()");
        return err;
    }

    tm->tvm_sec = (uint32_t)tv.tv_sec;
    tm->tvm_usec = (uint32_t)tv.tv_usec;

    return err;
    
}

struct tm *get_localtime(time_t *t)
{
    time(t);
    return localtime(t);
}

struct tm *get_utctime(time_t *t)
{
    time(t);
    return gmtime(t);
}

int time_2_str(char *str, int max_size, const char *format, const struct tm *p_tm)
{
    int err = 0;
    
    if (NULL == str)
        err = 0;
    if (0 == (err = strftime(str, max_size, format, p_tm)))
    {
        perror("time_2_str()");
        goto end;
    }

end:
    return err; 
}

int get_str_time(char *str, int max_size, const char *format, time_type tt)
{
    int err = 0;
    time_t t;
    struct tm *p_tm = NULL;

    if ((NULL == str) || (0 >= max_size))
        goto end;

    if (LOCAL_TIME == tt)
        p_tm = get_localtime(&t);
    else if (UTC_TIME == tt)
        p_tm = get_utctime(&t);
    else
        goto end;

    err = time_2_str(str, max_size, format, p_tm);
    
end:
    return err;
}


FILE *open_file(char *path, char *mode)
{
    FILE *f = NULL;

    if ((NULL == path) || (NULL == mode))
        goto end;
    if (NULL == (f = fopen(path, mode)))
        perror("open_file()");
    goto end;

end:
    return f;
}

void close_file(FILE *p)
{
    if (NULL != p)
    {
        fclose(p);
        p = NULL;
    }   
}

int file_write(void *p, size_t size, size_t mem_num, FILE *f)
{
    int err = 0;

    if ((NULL == p) || (NULL == f))
    {
        err = -1;
        goto end;
    }

    if (0 >= (err = fwrite(p, size, mem_num, f)))
    {
        perror("file_write()");
        goto end;
    }

end:
    return err;
}

int file_read(void *p, size_t size, size_t mem_num, FILE *f)
{
    int err = 0;

    if ((NULL == p) || (NULL == f) || (0 >= size) || (0 >= mem_num))
    {
        err = -1;
        goto end;
    }

    if (0 >= (err = fread(p, size, mem_num, f)))
    {
        perror("file_write()");
        goto end;
    }

end:
    return err;
}

/* execute shell command with system() */
int exec_command_1(char *cmd)
{
    int err = 0;
    
    if (0 != (err = system(cmd)))
        perror("exec_command_1()");
    
    return err;
}

int dir_exist(const char *name)
{
    int res = 1;
    DIR *d = NULL;

    if (NULL == name)
        res = 0;
    
    if (NULL == (d = opendir(name)))
    {
        res = 0;
    }
    else
    {
        closedir(d);
    }

    return res;
}

