/*
NAME		: Santhoshkumar R
DATE		: 30-09-2023
DESCRIPTION	: Steganography Project
*/

/* Including header files */
#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* Function Definition For Decoding */

/* Read and validate decode args from argv (Command line arguments) */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	// Validating source file extension
	if( strcmp((strstr(argv[2], ".")), ".bmp") == 0 )
	{
		decInfo -> stego_fname = argv[2];

		printf("INFO : SUCCESS --> Soruce File extension checking\n");
	}
	else
	{
		return e_failure;
	}

	// Validating if Output file name is given
	if( argv[3] != NULL)
	{

		int index = 0;

		// Store output file name in an array
		while(argv[3][index])
		{
			decInfo -> secret_fname[index] = argv[3][index];

			index++;
		}

		decInfo -> secret_fname[index] = '\0';

	}
	else
	{

		// If user not passed output file name create default file name 
		char *sec_name = "secret_decode";

		int index = 0;

		while(sec_name[index])
		{
			decInfo -> secret_fname[index] = sec_name[index];

			index++;
		}

		decInfo -> secret_fname[index] = '\0';
	}

	return e_success;
}

/* Perfrom the Decoding */
Status do_decoding(DecodeInfo *decInfo)
{
	// Opening source Image file
	decInfo -> fptr_stego_fname = fopen(decInfo -> stego_fname, "r");

	if(decInfo -> fptr_stego_fname == NULL)
	{
		perror("fopen");

		fprintf(stderr, "ERROR : Unable to open file %s\n", decInfo -> stego_fname);

		return e_failure;
	}

	// Function Calling -->  Decoding Magic string 
	if(decode_magic_string(decInfo) == e_success)
	{
	    printf("INFO : SUCCESS --> MAGIC STRING Matched\n");
	}
	else
	{
		printf("INFO : SUCCESS --> MAGIC STRING not Matching\n");

		return e_failure;
	}

	// Function Calling --> Decoding Secret file extension size
	if(decInfo -> extn_secret_size = (decode_lsb_bit_into_integer(decInfo -> fptr_stego_fname)))
	{
		printf("INFO : SUCCESS --> Secret file extension size Decoded\n");
	}

	// Function Calling --> Decoding Secret file extension
	if(decode_secret_file_extn(decInfo -> extn_secret_size, decInfo) == e_success)
	{
		printf("INFO : SUCCESS --> Secret file Extension Decoded\n");
	}

	// Opening the secret file with Write mode
	decInfo -> fptr_secret_fname = fopen(decInfo -> secret_fname, "w");

	if(decInfo -> fptr_secret_fname == NULL)
	{
		perror("fopen");

		fprintf(stderr, "ERROR : Unable to open file %s\n", decInfo -> secret_fname);

		return e_failure;
	}
	else
	{
		printf("INFO : SUCCESS --> Output Image file is Created\n");
	}

	// Function Calling --> Decoding secret file size
	if(decInfo -> secret_file_size = decode_lsb_bit_into_integer(decInfo -> fptr_stego_fname))
	{
		printf("INFO : SUCCESS --> Secret file size Decoded\n");
	}

	// Function Calling --> Decoding Secret data
	if(decode_secret_data(decInfo -> secret_file_size, decInfo) == e_success)
	{

		printf("INFO : SUCCESS --> Secret data Decoded\n");

	}
	else
	{
		printf("INFO : FAILURE --> Secret data Decoded\n");
	}

	return e_success;

}

/* Decoding Magic String from source Image */
Status decode_magic_string(DecodeInfo *decInfo)
{
	
	char magic[3] = {0};

	char buffer[8];

	fseek(decInfo -> fptr_stego_fname, 54, SEEK_SET);

	for(int index = 0 ; index < 2 ; index++)
	{
		fread(buffer, 1, 8, decInfo -> fptr_stego_fname);

		// Function Calling  --> To decode magic string
		magic[index] = decode_lsb_bit_into_character(magic[index], buffer);

	}

	magic[3] = '\0';

	printf("INFO : SUCCESS --> MAGIC STRING Decoded\n");

	// Checking Magic string are equal
	if(strcmp(magic,MAGIC_STRING) == 0)
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}

}

/* Decoding Secret file extension from source Image */
Status decode_secret_file_extn(int size, DecodeInfo *decInfo)
{	
	char extn_secret[size+1];
	char buffer[8];
	int index;

	for(index = 0 ; index < size ; index++)
	{
		extn_secret[index] = 0;

		fread(buffer, 1, 8, decInfo -> fptr_stego_fname);

		// Function Calling --> To decode the secret file extn
		extn_secret[index] = decode_lsb_bit_into_character(extn_secret[index], buffer);
	}
	extn_secret[size] = '\0';

	// Concatenating Secret file name with decoded extension
	strcat( decInfo -> secret_fname, extn_secret);
	return e_success;

}

/* Decoding secret file data from source Image */
Status decode_secret_data(int size, DecodeInfo *decInfo)
{
	char secret_data[size];

	char buffer[8];

	for(int index = 0 ; index < size ; index++)
	{
		secret_data[index] = 0;

		fread(buffer, 1, 8, decInfo -> fptr_stego_fname);

		// Function calling --> To decode secret data into character
		secret_data[index] = decode_lsb_bit_into_character(secret_data[index], buffer);
	}

	fwrite(secret_data, 1, size, decInfo -> fptr_secret_fname);

	return e_success;
}

/*  Decoding LSB bit converting to Integer */
uint decode_lsb_bit_into_integer(FILE *fptr_stego_fname)
{
	char buffer[32];

	unsigned int digit = 0;

	fread(buffer, 1, 32, fptr_stego_fname);

	// Logic for converting LSB bit into Integer
	for(int index = 0 ; index < 32 ; index++)
	{
		digit = (((buffer[index] & 1) << (31 - index) ) | digit);
	}

	return digit;
}

/* Decoding LSB bit converting to character */
char decode_lsb_bit_into_character(char data, char *buffer)
{
	// Logic for Converting LSB bit into character
	for(int index = 0 ; index < 8 ; index++)
	{
		data = (((buffer[index] & 1) << (7-index) ) | data);
	}
	return data;
}
