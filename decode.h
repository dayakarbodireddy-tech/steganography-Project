#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    
    
    
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char secret_fname[20];
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX + 1];
    
    long size_secret_file;

   

} DecodeInfo;


/* Encoding function prototype */

/* Check operation type */


/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);




/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status skip_copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(uint size, DecodeInfo *decInfo);
/* Encode secret file extension size */
Status decode_secret_file_extn_size(uint *size, DecodeInfo *decInfo);

/* Encode size into LSB of image data */
Status decode_size_to_lsb(uint size, char *image_buffer);

/* Encode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);


/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Encode function, which does the real encoding */
Status decode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status decode_byte_to_lsb(char *image_buffer, char *data);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
