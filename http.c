#include "http.h"

/*parse hostname and path from url
 * return 0 for success
 * -1 for fail
 * */
int parse_url(char *url, char **host, char **path){
  char *p = strstr(url, "//");
  if(!p)
    return -1;

  *host = p + 2;
  p = strchr(*host, '/');
  if(!p)
    *path = NULL;
  else{
    *path = p + 1;
    *p = '\0';
  }
  return 0;
}

int main(int argc, char **argv){
  char *host, *path;

  if(argc !=2){
    printf("Usage: %s URL\n", argv[0]);
    exit(-1);
  }

  if(parse_url(argv[1], &host, &path)){
    printf("can not parse host name and path from %s\n", argv[1]);
    exit(-1);
  }

  printf("connecting to %s/%s\n", host, path);

  return 0;
}
