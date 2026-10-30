#include <stdio.h>
#include<string.h>
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
// Status open_files(EncodeInfo *encInfo)
//  {
//     // Src Image file
//     encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
//     // Do Error handling
//     if (encInfo->fptr_src_image == NULL)
//     {
//     	perror("fopen");
//     	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

//     	return e_failure;
//     }

//     // Secret file
//     encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
//     // Do Error handling
//     if (encInfo->fptr_secret == NULL)
//     {
//     	perror("fopen");
//     	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

//     	return e_failure;
//     }

//     //Stego Image file
//     encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
//     // Do Error handling
//     if (encInfo->fptr_stego_image == NULL)
//     {
//     	perror("fopen");
//     	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

//     	return e_failure;
//     }

//     // No failure return e_success
//     return e_success;
// }
Status read_and_validate_encode_args(int argc,char *argv[],EncodeInfo *encInfo)
{
    if(argc < 4 || argc > 5)
    {
        return e_failure;
    }
    int len=strlen(argv[2]);
    if(len >= 4 && strcmp(argv[2] + len - 4,".bmp")==0)
    {
       encInfo-> src_image_fname=argv[2];
      encInfo -> fptr_src_image=fopen(encInfo -> src_image_fname,"rb");
      if(encInfo -> fptr_src_image==NULL)
      {
          return e_failure;
        }
    }else{
        printf("Error:Source image must have .bmp extension\n");
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
        printf("Error:Secret file not having the valid extension\n");
        return e_failure;
    }
    
    if(argc==5)
    {
        int len=strlen(argv[4]);
        if(len >= 4 && strcmp(argv[4] + len - 4,".bmp")==0)
        {
        encInfo -> stego_image_fname = argv[4];
        }else{
            printf("Error:Output image doenot copy the source image header 54 bytes\n");
            return e_failure;
        }
    }else{
        encInfo -> stego_image_fname ="default.bmp";
        
    }
    encInfo -> fptr_stego_image=fopen(encInfo -> stego_image_fname,"wb");
    {
        if(encInfo -> fptr_stego_image ==NULL)
        {
            printf("Error:unable to open the output image(stego image)\n");
            return e_failure;
        }
    }
    return e_success;
        
        
        
        

}
Status do_encoding(EncodeInfo *encInfo)
{
  if(check_capacity(encInfo)==e_failure)
  {
    printf("Secret file have more capacity compared to source image file\n");
    return e_failure;
  }
  if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)==e_failure)
  {
    printf("File header not copied into the destination file\n");
    return e_failure;
  }
  if(encode_magic_string(MAGIC_STRING, encInfo)==e_failure)
  {
    printf("source image doesnot provide 8 bytes data to store magic string\n");
    return e_failure;
  }
  char *dot=strrchr(encInfo -> secret_fname,'.');
  if(dot==NULL)
  {
    printf("Secret file extension is not found\n");
    return e_failure;
  }
  strcpy(encInfo->extn_secret_file,dot);
  if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo)==e_failure)
  {
    printf("Secret file extension is not valid\n");
    return e_failure;
  }
  if(encode_secret_file_extn(encInfo -> extn_secret_file,encInfo)==e_failure)
  {
    printf("Secret file extension is not encoded successfully in source image\n ");
    return e_failure;
  }
  if(encode_secret_file_size(encInfo -> size_secret_file,encInfo)==e_failure)
  {
    printf("Encoding secret file size in source image is failed\n");
    return e_failure;
  }
  if(encode_secret_file_data(encInfo)==e_failure)
  {
    printf("Secret file actual contents not hide in output image\n");
    return e_failure;
  }
  if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo ->fptr_stego_image)==e_failure)
  {
    printf("source bytes  will not copied successfully inside output stego image\n");
    return e_failure;
  }
  return e_success;
}
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> size_secret_file =get_file_size(encInfo -> fptr_secret);
    if(get_image_size_for_bmp(encInfo -> fptr_src_image)>=((2+4+4+4)*8) + (encInfo -> size_secret_file * 8))
    {
        return e_success;
    }else{
        return e_failure;
    }
}
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    uint size=ftell(fptr);
    rewind(fptr);
    return size;
}
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_stego_image)
{
    char buff[54];
    rewind(fptr_src_image);
    if(fread(buff,sizeof(buff[0]),54,fptr_src_image)!=54)
    {
        return e_failure;
    }
    if(fwrite(buff,sizeof(buff[0]),54,fptr_stego_image)!=54)
    {
        return e_failure;
    }
    return e_success;
}
Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo)
{
    char image_buffer[8];
    for(int i=0;i<2;i++)
    {
        if(fread(image_buffer,sizeof(image_buffer[0]),8,encInfo -> fptr_src_image)!=8)
        {
            return e_failure;
        }
        encode_byte_to_lsb(magic_string[i],image_buffer);
        if(fwrite(image_buffer,sizeof(image_buffer[0]),8,encInfo -> fptr_stego_image)!=8)
        {
            return e_failure;
        }

        
    }
    return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
      int bit=(data>>i) & 1;
      image_buffer[i]=image_buffer[i] & 0xFE;
      image_buffer[i]=image_buffer[i] | bit;
    }
    return e_success;
}
Status encode_secret_file_extn_size(uint size, EncodeInfo *encInfo)
{
    char image_buffer[32];
    if(fread(image_buffer,sizeof(image_buffer[0]),32,encInfo -> fptr_src_image)!=32)
    {
       return e_failure;
    }
    encode_size_to_lsb(size,image_buffer);
    if(fwrite(image_buffer,sizeof(image_buffer[0]),32,encInfo -> fptr_stego_image)!=32)
    {
        return e_failure;
    }
    return e_success;
}
Status encode_size_to_lsb(uint size,char *image_buffer)
{
    for(int i=0;i<32;i++)
    {
        int bits=(size>>i) & 1;
        image_buffer[i]=image_buffer[i] & 0xFE;
        image_buffer[i]=image_buffer[i] | bits;
    }
    return e_success;
}
Status encode_secret_file_extn(const char *file_extn,EncodeInfo *encInfo)
{
    char image_buffer[8];
    for(int i=0;file_extn[i]!='\0';i++)
    {
        if(fread(image_buffer,sizeof(image_buffer[0]),8,encInfo -> fptr_src_image)!=8)
        {
            return e_failure;
        }
        encode_byte_to_lsb(file_extn[i],image_buffer);
        if(fwrite(image_buffer,sizeof(image_buffer[0]),8,encInfo -> fptr_stego_image)!=8)
        {
            return e_failure;
        }
    }
    return e_success;
}
Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
    char image_buffer[32];
    if(fread(image_buffer,sizeof(image_buffer[0]),32,encInfo -> fptr_src_image)!=32)
    {
        return e_failure;
    }
    encode_size_to_lsb(file_size,image_buffer);
    if(fwrite(image_buffer,sizeof(image_buffer[0]),32,encInfo -> fptr_stego_image)!=32)
    {
        return e_failure;
    }
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char image_buffer[8],data;
    for(int i=0;i<encInfo -> size_secret_file;i++)
    {
        if(fread(image_buffer,sizeof(image_buffer[0]),8,encInfo -> fptr_src_image)!=8)
        {
            return e_failure;
        }
        if(fread(&data,sizeof(image_buffer[0]),1,encInfo -> fptr_secret)!=1)
        {
            return e_failure;
        }
        encode_byte_to_lsb(data,image_buffer);
        if(fwrite(image_buffer,sizeof(image_buffer[0]),8,encInfo -> fptr_stego_image)!=8)
        {
            return e_failure;
        }
    }
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src_image,FILE *fptr_stego_image)
{
    int ch;
    while((ch=fgetc(fptr_src_image))!=EOF)
    {
       if(putc(ch,fptr_stego_image)==EOF)
       {
        return e_failure;
       }
    }
    if(ferror(fptr_src_image))
    {
        return e_failure;
    }
    return e_success;
}


