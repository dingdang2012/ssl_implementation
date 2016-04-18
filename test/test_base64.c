#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "base64.h"

int main(int argc, char **argv){
  unsigned char *buf = NULL;

  if(argc != 3){
    printf("Usage: %s [-e|-d] string\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if(!strncmp(argv[1], "-e", 2)){ /*encoding*/
    buf = (unsigned char *)calloc(strlen(argv[2])*4/3 + 1, sizeof(unsigned char));
    assert(buf); 
    base64_encode((const unsigned char *)argv[2], strlen(argv[2]), buf);
    printf("%s\n", buf);
    free(buf), buf = NULL;
    return 0;
  }

  if(!strncmp(argv[1], "-d", 2)){ /*decoding*/
    buf = (unsigned char *)calloc(strlen(argv[2])*3/4 + 1, sizeof(unsigned char));
    assert(buf);
    base64_decode((const unsigned char *)argv[2], strlen(argv[2]), buf);
    printf("%s\n", buf);
    free(buf), buf = NULL;
  }

  return 0;
}
