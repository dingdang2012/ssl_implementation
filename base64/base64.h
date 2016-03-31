#include <stdio.h>
#include <string.h>
#include <assert.h>

void base64_encode(const unsigned char *input, int len, unsigned char *output);
int base64_decode(const unsigned char *input, int len, unsigned char *output);
