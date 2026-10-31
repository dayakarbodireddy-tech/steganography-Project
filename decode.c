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
 if(strstr(argv[1],".bmp")==NULL)
 {
    printf("Error:Stego image exetension is not valid\n");
    return e_failure;
 }
 decInfo -> src_image_fname =argv[2];
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
    
}