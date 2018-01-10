#ifndef _COMMON_H_
#define _COMMON_H_

#define LINE_DIS_NUM 16

//Display LINE_DIS_NUM data in hex in one line.
void hex_dump(char *data, int len);

#define DEBUG
#ifdef DEBUG
#define d_printf(s)
#endif //DEBUG


#endif //_COMMON_H_
