#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo)
{
  if(argc < 3 || argc > 4)
  {
    printf("Error:Argument count not should be more or lesss as compared to required\n");
    return e_failure;
  }
 if(strcmp(argv[1],"-d")!=0)
 {
    printf("Error:Choosed operation type is wrong\n");
    return e_failure;
 }
 if(strstr(argv[2],".bmp")==NULL)
 {
    printf("Error:Stego image exetension is not valid\n");
    return e_failure;
 }
 decInfo -> src_image_fname =argv[2];
 decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "rb");

if(decInfo->fptr_src_image == NULL)
{
    printf("Unable to open stego image\n");
    return e_failure;
}
 if(argc == 4)
 {
    strcpy(decInfo -> secret_fname,argv[3]);
 }else{
    strcpy(decInfo -> secret_fname,"decoded");
 }
  return e_success;
}
Status do_decoding(DecodeInfo *decInfo)
{
  if(fseek(decInfo->fptr_src_image, 54, SEEK_SET) != 0)
  {
    printf("File position is not pointing from the 54th byte\n");
    return e_failure;
  }
  if(decode_magic_string(MAGIC_STRING, decInfo)==e_failure)
  {
   printf("Error:Decoded magic string is not matched with the original magic string\n");
    return e_failure;
  }
  uint extn_size;
  if(decode_secret_file_extn_size(&extn_size, decInfo)==e_failure)
  {
    printf("Error:Decoding the extension size is failed\n");
    return e_failure;
  }
  if(decode_secret_file_extn(extn_size, decInfo)==e_failure)
  {
    printf("Error:Decoding file extension is failed\n");
    return e_failure;
  }
   if(decode_secret_file_size(decInfo)==e_failure)
  {
    printf("Error:Decoding file size failed\n");
    return e_failure;
  }
  char output_fname[50];
  strcpy(output_fname,decInfo -> secret_fname);
  strcat(output_fname,decInfo -> extn_secret_file);
  decInfo -> fptr_secret = fopen(output_fname,"wb");
  if(decInfo -> fptr_secret == NULL)
  {
    printf("output file is not created\n");
    return e_failure;
  }
 
  if(decode_secret_file_data(decInfo)==e_failure)
  {
    printf("Error:Decoding file data failed\n");
    return e_failure;
  }
  fclose(decInfo -> fptr_src_image);
  fclose(decInfo -> fptr_secret);
  return e_success;
}
Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo)
{
    char image_buffer[8];
    char decoded_char;
    for(int i=0;i<2;i++)
    {
        if(fread(image_buffer,sizeof(image_buffer[0]),8,decInfo -> fptr_src_image)!=8)
        {
            return e_failure;
        }
        decode_byte_to_lsb(image_buffer,&decoded_char);
        if(decoded_char != magic_string[i])
        {
            printf("Error:Decoded magic string is not matched with the original magic string\n");
            return e_failure;
        }

        
    }
    return e_success;
}
Status decode_byte_to_lsb(char *image_buffer, char *data)
{
    *data=0;
    for(int i=0;i<8;i++)
    {
      int bit=image_buffer[i] & 1;
      *data = *data | (bit<<i);
    }
    return e_success;
}
Status decode_secret_file_extn_size(uint *size, DecodeInfo *decInfo)
{
    char image_buffer[32];
    if(fread(image_buffer,sizeof(image_buffer[0]),32,decInfo -> fptr_src_image)!=32)
    {
        return e_failure;
    }
   *size=0;
   for(int i=0;i<32;i++)
   {
    int bit = image_buffer[i] & 1;
    *size = *size |((uint)bit << i);
   }
    return e_success;
}
Status decode_secret_file_extn(uint size, DecodeInfo *decInfo)
{
    if(size > MAX_FILE_SUFFIX)
    {
        printf("Error:Invalid file extension size\n");
        return e_failure;
    }
    char image_buffer[8];
    for(int i=0;i<size;i++)
    {
        if(fread(image_buffer,sizeof(image_buffer[0]),8,decInfo -> fptr_src_image)!=8)
        {
            return e_failure;
        }
        decode_byte_to_lsb(image_buffer,&decInfo -> extn_secret_file[i]);
        
    }
    decInfo -> extn_secret_file[size]='\0';
    return e_success;
}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
  char image_buffer[32];
    if(fread(image_buffer,sizeof(image_buffer[0]),32,decInfo -> fptr_src_image)!=32)
    {
        return e_failure;
    }
    decInfo -> size_secret_file =0;
    for(int i=0;i<32;i++)
    {
        int bit=image_buffer[i] & 1;
        decInfo -> size_secret_file = decInfo -> size_secret_file | ((uint)bit<<i);
    }
    return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char image_buffer[8],data;
    for(int i=0;i<decInfo -> size_secret_file;i++)
    {
        if(fread(image_buffer,sizeof(image_buffer[0]),8,decInfo -> fptr_src_image)!=8)
        {
            return e_failure;
        }
        
        decode_byte_to_lsb(image_buffer,&data);
        if(fwrite(&data,sizeof(char),1,decInfo -> fptr_secret)!=1)
        {
            return e_failure;
        }
    }
    
    return e_success;
}
