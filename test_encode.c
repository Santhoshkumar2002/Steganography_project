/*
NAME		: Santhoshkumar R
DATE		: 30-09-2023
DESCRIPTION	: Steganography Project
*/


/* Including header files */
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main( int argc, char *argv[])
{
	// Declaring two structure variables
    EncodeInfo encInfo;

	DecodeInfo decInfo;

	// Validate whether CLI is passed
	if(argc < 2)
	{
		printf("ERROR : Please Pass Command Line Arguments\n");
		return 0;
	}

	/* Validate which Operation have to be done */
	// Function Calling --> For encode operation
	if(check_operation_type(argv) == e_encode)
	{
		printf("INFO : Selected Encoding\n");
		printf("------>>Encoding<<------\n");

		// Validate required number of CLI is passed for encoding
		if( argc >= 4 && argc <= 5)
		{
			// Function Calling --> For read and validate the CLI arguments
			if( read_and_validate_encode_args(argv, &encInfo) == e_success)
			{
				printf("INFO : SUCCESS --> Read and Validate Done\n");
			}
			else
			{
				printf("INFO : FAILURE --> Error Read and Valiadte\n");
				return e_failure;
			}

			// Function Calling --> For Encoding the Secret data
			if ( do_encoding(&encInfo) == e_success)
			{
				printf("------>>Encoding is SUCCESSFULL<<------\n");
			}
			else
			{
				printf("------>>Encoding is FAILURE<<------\n");
				return e_failure;
			}

		}
		else
		{
			printf("INFO : ERROR --> Less arguments passed for Encoding in Command Line Arguments\n");
			printf("Please pass needed arguments For Encoding\n");
		}

	}
	// Function Calling --> For decode operation
	else if( check_operation_type(argv) == e_decode)
	{
		printf("INFO : Selected Decoding\n");
		printf("------>>Decoding<<------\n");

		// Validate required number of CLI is passed for decoding
		if(argc >=3 && argc <= 4)
		{
			// Function Calling --> For read and validate the CLI arguments
			if(read_and_validate_decode_args(argv, &decInfo) == e_success)
			{
				printf("INFO : SUCCESS --> Read and validate Done\n");
			}
			else
			{
				printf("INFO : FAILURE --> Error Read and validate\n");
			}

			// Function Calling --> For decoding Secret data from source Image
			if(do_decoding(&decInfo) == e_success)
			{
				printf("------>>Decoding is SUCCESSFULL<<------\n");
			}
			else
			{
				printf("------>>Decoding is FAILURE<<------\n");
			}

		}
		else
		{
			printf("Error : Less arguments passed for Decoding in Command Line Arguments\n");
			printf("Please pass needed arguments for Decoding\n");
		}


	}
	else
	{
		printf("INFO : ERROR --> Unsupported Type\n");
		printf("INFO : -e --> ENCODING\n");
		printf("INFO : -d --> DECODING\n");
		printf("EXAMPLE : ./a.out -e beautiful.bmp secret.txt\n");
	}

    return 0;
}

/* check which operation type to be perform */
OperationType check_operation_type(char *argv[])
{
	// Validate if encoding
	if( (strcmp( argv[1], "-e")) == 0 )
	{
		return e_encode;
	}
	// Validate if decoding
	else if( (strcmp( argv[1], "-d")) == 0 )
	{
		return e_decode;
	}
	else
	{
		return e_unsupported;
	}
}
