/*
NAME 		: Santhoshkumar R
DATE		: 30-09-2023
DESCRIPTION	: Steganography
*/

/* Including header files */
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
	uint width, height;
	// Seek to 18th byte
	fseek(fptr_image, 18, SEEK_SET);

	// Read the width (an int)
	fread(&width, sizeof(int), 1, fptr_image);
	printf("width = %u\n", width);

	// Read the height (an int)
	fread(&height, sizeof(int), 1, fptr_image);
	printf("height = %u\n", height);

	// Return image capacity
	return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

// Open files Function
Status open_files(EncodeInfo *encInfo)
{
	// Src Image file
	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
	// Do Error handling
	if (encInfo->fptr_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

		return e_failure;
	}

	// Secret file
	encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
	// Do Error handling
	if (encInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

		return e_failure;
	}

	// Stego Image file
	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
	// Do Error handling
	if (encInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

		return e_failure;
	}

	// No failure return e_success
	return e_success;
}

// Read and validate Encode args from argv (Command line arguments)
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	//Validating the soruce file extension 
	if(strcmp((strstr(argv[2], ".")) , ".bmp") == 0 )
	{
		// Storing the source file name
		encInfo -> src_image_fname = argv[2];

		printf("INFO : SUCCESS --> Source Extension checked\n");
	}
	else
	{
		return e_failure;
	}

	// Possible secret file extensions
	char * extent[4] = {".sh", ".txt", ".c", ".h"};

	int index, count = 0;

	// Validating the secret file extension
	for( index = 0 ; index < 4 ; index++)
	{
		if(strcmp((strstr(argv[3],".")) ,extent[index]) == 0 )
		{
			count = 1;
			break;
		}
	}

	// Validating that possible extension is persent 
	if( count != 0 )
	{
		// Storing the secret file name
		encInfo -> secret_fname = argv[3];

		char *extent = strstr(encInfo -> secret_fname, ".");

		int extn_size = 0;

		while(extent[extn_size])
		{
			// Storing the extension of the secret file
			encInfo -> extn_secret_file[extn_size] = extent[extn_size];

			extn_size++;
		}

		// Storing the extension size of secret file
		encInfo -> extn_secret_size = extn_size;

		printf("INFO : SUCCESS --> Storing  Secret file Extension is Done\n");


	}
	else
	{
		return e_failure;
	}

	// Validating if output file name is not given 
	if( argv[4] == NULL )
	{
		// Creating output file name and Storing the output file name
		encInfo -> stego_image_fname = "stego.bmp";
	}
	else
	{
		// If the output file name is mentioned store that File name
		if(strcmp((strstr(argv[4], ".")), ".bmp") == 0)
		{
			encInfo -> stego_image_fname = argv[4];
		}
		else
		{
			return e_failure;
		}
	}

	return e_success;

}

// Perform the Encoding 
Status do_encoding(EncodeInfo *encInfo)
{
	// Function calling --> Opening the requried files
	if( open_files(encInfo) == e_success)
	{
		printf("INFO : SUCCESS --> Opening the files\n");
	}
	else
	{
		printf("INFO : FAILURE --> Opening the files\n");

		return e_failure;
	}

	// Function Calling --> To check the capacity of the source file for encoding
	if(check_capacity(encInfo) == e_success)
	{
		printf("INFO : SUCCESS --> Capacity of the Source Image is Checked\n");
	}
	else
	{
		printf("INFO : FAILURE --> Capacity Checking\n");

		return e_failure;
	}

	// FUnction Calling --> Storing header content of source Image file into output Image file
	if( copy_bmp_header( encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
	{
		printf("INFO : SUCCESS --> Header in Source file is copied to resultant Image file\n");
	}
	else
	{
		printf("INFO : FAILURE --> Header in Source file is failed to copy to output file\n");

		return e_failure;
	}

	// Function Calling --> Encoding magic string into output Image file
	if( encode_magic_string(MAGIC_STRING, encInfo) == e_success)
	{
		printf("INFO : SUCCESS --> MAGIC STRING Encoded\n");
	}

	// Function Calling --> Encoding secret file extension file into output Image file
	if(encode_secret_file_extn_size( encInfo -> extn_secret_size, encInfo) == e_success)
	{
		printf("INFO : SUCCESS --> Secret file extension size Encoded\n");
	}

	// Function Calling --> Encoding secret file extn into output Image file
	if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
	{
		printf("INFO : SUCCESS --> Secret file extension Encoded\n");
	}

	// Function Calling --> Encoding Secret file size into Output Image file
	if(encode_secret_file_size( encInfo -> size_secret_file , encInfo) == e_success)
	{
		printf("INFO : SUCCESS --> Secret file size Encoded\n");
	}

	// Function Calling --> Encoding Secret data into Output Image file
	if(encode_secret_file_data(encInfo) == e_success)
	{
		printf("INFO : SUCCESS --> Secret data Encoded\n");
	}

	// Function Calling --> Copying remaining Image data into Output Image file
	if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
	{
		
		printf("INFO : SUCCESS --> Remaining data copied to Output Image File\n");
	}

	return e_success;
}

/* check Capacity */
Status check_capacity(EncodeInfo *encInfo)
{
	encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);

	encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);

	// Validating the Image Capacity to hide secret data
	if( (encInfo -> image_capacity - 54 ) >= (strlen(MAGIC_STRING)+ 4 + encInfo -> extn_secret_size + 4 + encInfo -> size_secret_file) * 8)
	{
		return e_success;
	}

	return e_failure;
}

/* get file size */
uint get_file_size(FILE *fptr)
{
	char offset;

	while( (offset = fgetc(fptr)) != EOF);

	return ftell(fptr);

}

/* Copy bmp image header to Output Image*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{

	char header_content[54];

	rewind(fptr_src_image);

   	fread( header_content, 1, 54, fptr_src_image );

	fwrite( header_content, 1, 54, fptr_dest_image);

	return e_success;
}

/* Store Encoded magic string into output Image file */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{

	int index = 0;

	while(magic_string[index])
	{
		// Read 8 bytes image data from souce Image file for encoding magic string
		fread( encInfo -> image_data, 1, 8, encInfo -> fptr_src_image);

		// Function Calling --> For encoding the Magic String 
		encode_byte_to_lsb(magic_string[index], encInfo -> image_data);

		// Store the encoded image data to Output Image file
		fwrite( encInfo -> image_data, 1, 8, encInfo -> fptr_stego_image);

		index++;
	}

	return e_success;
}

/* Encode secret file extn size and Store Encoded Image data Into Output Image */
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
	char buffer[32];

	// Read 32 bytes Image data from source Image file for encoding secret file extn size
	fread( buffer, 1, 32, encInfo -> fptr_src_image);

	// Encoding the secret file extn size in 32 byte image data
	for(int index = 0 ; index < 32 ; index++)
	{
		buffer[index] = ((buffer[index] & 0xFE) | ( (unsigned)( extn_size & (1 << (31-index)) ) >> (31-index) ));

	}

	// Store the encoded Image data into Output Image file
	fwrite( buffer, 1, 32, encInfo -> fptr_stego_image);

	return e_success;
}

/* Encoding secret file extension into Output Image file */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	char buffer[8];

	int index = 0;

	while(index < encInfo -> extn_secret_size)
	{
		fread(buffer , 1, 8, encInfo -> fptr_src_image);

		// Function calling --> For Encoding secret file extension
		encode_byte_to_lsb(file_extn[index], buffer);

		fwrite(buffer, 1, 8, encInfo -> fptr_stego_image);

		index++;
	}

	return e_success;
}

/* Encoding Secret file extension into Ouput Image file */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
	char buffer[32];

	// Read 32 bytes data from Source Image data
	fread( buffer, 1, 32, encInfo -> fptr_src_image);

	// Encoding the secret file size in 32 byte image data
	for(int index = 0 ; index < 32 ; index++)
	{
		buffer[index] = ((buffer[index] & 0xFE) | ( (unsigned)( file_size & (1 << (31-index)) ) >> (31-index) ));

	}

	// Store the encoded Image data into Output Image file
	fwrite( buffer, 1, 32, encInfo -> fptr_stego_image);

	return e_success;
}

/* Encoding secret file data into Output Image file */
Status encode_secret_file_data(EncodeInfo *encInfo)
{

	char char_data = 0;

	fseek(encInfo -> fptr_secret, 0L, SEEK_SET);

	// Run the loop for size of the secret file times
	for(int index = 0; index < encInfo -> size_secret_file ; index++)
	{
		// Read data from secret file 
		fread(&char_data, 1, 1,encInfo -> fptr_secret);

		// Read 8 bytes Image data from the source Image
		fread(encInfo -> image_data, 1, 8, encInfo -> fptr_src_image);

		// Function calling --> For encoding secret data
		encode_byte_to_lsb(char_data, encInfo -> image_data);

		fwrite(encInfo -> image_data, 1, 8, encInfo -> fptr_stego_image);
	}

	return e_success;

}

/* Encode a byte into LSB of Image data array */
Status encode_byte_to_lsb( char data, char *image_buffer)
{
	for(int index = 0 ; index < 8 ; index++)
	{
		// Logic for Encoding bit into Source image byte
		image_buffer[index] = ((image_buffer[index] & 0xFE) | ( (unsigned)( data & (1 << (7-index)) ) >> (7-index) ));

	}

	return e_success;
}

/* Copy remaining Image bytes from source to Ouput Image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	char remain_data;

	// Read Image data byte by byte from source Image 
	while(fread(&remain_data, 1, 1, fptr_src))
	{
		// Store Image data byte by byte to Output Image file
		fwrite(&remain_data, 1, 1, fptr_dest);
	}

	return e_success;
}
