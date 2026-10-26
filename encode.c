#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"

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
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    
    int len=strlen(argv[2]);
    if(len>4 && strcmp(argv[2]+len-4,".bmp")==0)
    {
       encInfo-> src_image_fname=argv[2];
      encInfo -> fptr_src_image=fopen(encInfo -> src_image_fname,"rb");
      if(encInfo -> fptr_src_image==NULL)
      {
        return e_failure;
      }
    }else{
        return e_failure;
    }
    
    if(argv[3] != NULL)
    {
      encInfo -> secret_fname = argv[3];
      encInfo -> fptr_secret = fopen(encInfo -> secret_fname,"rb");
      {
        if(encInfo -> fptr_secret==NULL)
        {
            return e_failure;
        }
      }
    }else{
        return e_failure;
    }
    
    if(argv[4]!=NULL)
    {
        int len=strlen(argv[4]);
        if(len>4 && strcmp(argv[4]+len-4,".bmp")==0)
        {
        encInfo -> stego_image_fname = argv[4];
        }else{
            return e_failure;
        }
    }else{
        encInfo -> stego_image_fname ="default.bmp";
        
    }
    return e_success;
        
        
        
        

}
Status do_ 
