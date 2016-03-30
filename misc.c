#include "common.h"

char *str_itoa(int i){
  char *s = calloc(10, sizeof(int));
  if(!s)
    return NULL;
  snprintf(s, sizeof(s), "%d", i);
  return s;
}
