#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    if(argc<2)
    {
        printf("PLEASE PROVIDE FILES\n");
        return 1;
    }
    if(check_operation_type(argv[1][1])==e_encode)
    {
       if(read_and_validate_encode_args(argv,&encInfo)==e_success)
       {
        printf("Arguments validated successfully\n");

       }
    }
    else if(check_operation_type(argv[1][1])==d_decode)
    {
        printf("Decode selected\n");
    }else{
        printf("UNSUPPORTED INFORMATION\n");
        return 1;
    }
    

    return 0;
}
OperationType check_operation_type(char opt)
{
  
  if(opt=='e')
  {
    return e_encode;
  }
  else if(opt=='d')
  {
    return d_decode;
  }
  else{
    return e_unsupported;
  }
}
