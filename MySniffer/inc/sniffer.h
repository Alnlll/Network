#ifndef _SNIFFER_H_
#define _SNIFFER_H_

#include "log.h"

typedef struct _sniff_info
{
    char *ifname;
    log log_ops;
} sniff_info;

/* 
 *Parameters parser
*/
//parameters parser definations
#define SNIFF_PARA_FORMAT "d:f:"

//parameters parser functions
int para_parser(sniff_info *info, int argc, char *argv[]);

/*
 * .cap files parser
*/
//.cap files parser definations
#define CAP_FILE_HDR_MAGIC 0xa1b2c3d4
#define CAP_FILE_HDR_SNAPLEN 0xffffu
#define CAP_FILE_HDR_MAJOR_VER 0x02
#define CAP_FILE_HDR_MINOR_VER 0x04
#define CAP_FILE_HDR_T_ZONE 0x00
#define CAP_FILE_HDR_SIGFIG 0x00

//.cap files link types
#define CAP_FILE_HDR_LNKTYPE_BSD 0x00
#define CAP_FILE_HDR_LNKTYPE_ETH 0x01
#define CAP_FILE_HDR_LNKTYPE_RAWIP 0x65
#define CAP_FILE_HDR_LNKTYPE_80211 0x69

//.cap files parser structures
typedef struct _cap_file_hdr
{
    uint32_t magic;  //default value: 0xa1b2c3d4
    unsigned short version_major; //major version number, default value:0x02
    unsigned short version_minor; //minor version number, default value:0x04
    int32_t thiszone; //not used
    uint32_t sigfigs; //not used
    uint32_t snaplen; //max packet len in byte
    uint32_t linktype; //link layer type.Ethernet:0x01,PPP:0x09,rawip:0x65,802.11:0x69
} cap_file_hdr;

typedef struct _cap_pkt_hdr
{
    timeval_m ts;
    uint32_t caplen;
    uint32_t len;
} cap_pkt_hdr;

//.cap files parser function
void cap_file_hdr_init(cap_file_hdr *head);

#endif //_SNIFFER_H_
