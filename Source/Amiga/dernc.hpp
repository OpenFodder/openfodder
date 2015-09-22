/*
 * dernc.h   define exported routines from dernc.c
 */

/**
 * DERNC From: https://github.com/alexandergitter/THViewer
 */

#ifndef RNC_DERNC_H
#define RNC_DERNC_H

/*
 * Routines
 */
long rnc_ulen (void *packed);
#ifndef COMPRESSOR
long rnc_unpack (void *packed, void *unpacked);
#else
long rnc_unpack (void *packed, void *unpacked, long *leeway);
#endif
const char *rnc_error (long errcode);
long rnc_crc (void *data, long len);

/*
 * Error returns
 */
#define RNC_FILE_IS_NOT_RNC    -1
#define RNC_HUF_DECODE_ERROR   -2
#define RNC_FILE_SIZE_MISMATCH -3
#define RNC_PACKED_CRC_ERROR   -4
#define RNC_UNPACKED_CRC_ERROR -5

/*
 * The compressor needs this define
 */
#define RNC_SIGNATURE 0x524E4301       /* "RNC\001" */

#endif
