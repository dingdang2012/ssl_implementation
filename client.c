#include "common.h"

int main(int argc, char **argv){
  char *host, *path;
  int fd, errn;
  struct addrinfo hints, *result, *rp;

  if(argc !=2){
    printf("Usage: %s URL\n", argv[0]);
    exit(-1);
  }

  if(parse_url(argv[1], &host, &path)){
    printf("can not parse host name and path from %s\n", argv[1]);
    exit(-1);
  }

  printf("connecting to %s/%s\n", host, path);
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  /*gethostbyname is deprecated, use getaddrinfo instead*/
  errn = getaddrinfo(host, "80", &hints, &result);
  if(errn != 0){
    printf("getaddrinfo: %s\n", gai_strerror(errn));
    exit(EXIT_FAILURE);
  }

  for(rp = result; rp != NULL; rp = rp->ai_next){
    fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if(fd == -1){
      printf("Error when creating socket\n");
      continue;
    }
    if(connect(fd, rp->ai_addr, rp->ai_addrlen) != -1){
      printf("connect succeed\n");
      break;
    }
    close(fd);
  }
  if(rp == NULL){//No address succeeded
    printf("Could not connect\n");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(result); /*No longer needed*/

  printf("Retrieving document: '%s'\n", path);
  if(!send_http_get(fd, path, host))
    printf("Send http GET success\n");
  display_result(fd);

  close(fd);
  return 0;
}
