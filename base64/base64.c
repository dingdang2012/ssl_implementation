#include "base64.h"
const static unsigned char *base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/*base64 encoding
 *assume size of output is 4/3 of len + 1
 *The algorythem of base64 is devide input bytes to 6 bits
 *every 6 bits map to a printable character
 * */
void base64_encode(const unsigned char *input, int len, unsigned char *output){
  do{
    *output++ = base64[(input[0] & 0xFC) >> 2];

    /*only one byte*/
    if(len == 1){
      *output++ = base64[(input[0] & 0x03) << 4];
      *output++ = '=';
      *output++ = '=';
      break;
    }

    *output++ = base64[((input[0] & 0x03) << 4) | ((input[1] & 0xF0) >> 4)];
    /*two bytes only*/
    if(len == 2){
      *output++ = base64[(input[1] & 0x0F) << 2];
      *output++ = '=';
      break;
    }
    *output++ = base64[((input[1] & 0x0F) << 2) | ((input[2] & 0xC0) >> 6)];
    *output++ = base64[(input[2] & 0x3F)];
    input += 3;
  } while(len -= 3); 
  /*process each 3 bytes = 3*8 bits = 24bits encoded to 4 printable char*/
  *output = '\0';
}

static int char_to_index(const unsigned char c){
  unsigned char *tmp = NULL;
  tmp = strchr((const char *)base64, c);
  if(tmp)
    return (tmp - base64);
  else
    return -1;
}
/*base64 decoding function
 *len must be multiple of 4
 *return output length for decode success
 *-1 for fail
 * */
int base64_decode(const unsigned char *input, int len, unsigned char *output){
  int i, t1, t2, t3, t4, out_len = 0;
  /*len must be multiple of 4*/
  assert(!(len & 0x03));
  do{
    /*check for illegal characters*/
    for(i = 0; i < 3; i++){
      if(input[i] == '=')
        break;
      if(char_to_index(input[i]) == -1){
        printf("Invalid char [%c]\n", input[i]);
        return -1;
      }
    }
    t1 = char_to_index(input[0]);
    t2 = char_to_index(input[1]);
    *output++ = ((t1 & 0x3F) << 2) | ((t2 & 0x30) >> 4);
    out_len++;
    if(input[2] != '='){
      t3 = char_to_index(input[2]);
      *output++ = ((t2 & 0x0F) << 4) | ((t3 & 0x3C) >> 2);
      out_len++;
    }
    if(input[3] != '='){
      t4 = char_to_index(input[3]);
      *output++ = ((t3 & 0x03) << 6) | (t4 & 0x3F);
      out_len++; 
    }
    input += 4;
  }while(len -= 4);

  return out_len;
}
