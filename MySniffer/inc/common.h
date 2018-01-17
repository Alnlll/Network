#ifndef _COMMON_H_
#define _COMMON_H_

#define LINE_DIS_NUM 16
//Display LINE_DIS_NUM data in hex in one line.
void hex_dump(char *data, int len);

void *zelloc(size_t size);

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

#define DEBUG
#ifdef DEBUG
#define d_printf printf
#else
#define d_printf
#endif //DEBUG

#define LITTEL_ENDIAN //Byte order.

#endif //_COMMON_H_
