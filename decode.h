#ifndef DECODE_H
#define DECODE_H

#include "types.h"  //Contains user defined types

/*
 * Structure to store information required for
 * decoding source Image file to secret file
 * Info about secret file and srouce file is also stored
 */

typedef struct _DecodeInfo
{
	/*Source Image info*/
	char *stego_fname;
	FILE *fptr_stego_fname;

	/*Output file info*/
	char secret_fname[20];
	FILE *fptr_secret_fname;
	uint extn_secret_size;
	unsigned long secret_file_size;

} DecodeInfo;


/* Decoding function prototype */

/* Read and Validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Decoding the magic string from source Image */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decoding the secret file extension from source Image */
Status decode_secret_file_extn(int size, DecodeInfo *decInfo);

/* Decoding the secret data from source Image */
Status decode_secret_data(int size, DecodeInfo *decInfo);

/* Decoding the lsb bit coverting to character */
char decode_lsb_bit_into_character(char data, char *buffer);

/* Decoding the lsb bit coverting to Integer */
uint decode_lsb_bit_into_integer(FILE *fptr_stego_fname);

#endif
