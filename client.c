#include "common.h"

int main(int argc, char **argv){
  char *host, *path;
  int fd, errn;
  struct addrinfo hints, *result, *rp;
  char *proxy_host, *proxy_user, *proxy_pw;
  int proxy_port, ind;
  char *port_str = NULL;

  if(argc < 2){
    printf("Usage: %s [-p http://[username:password@]proxy-host:proxy-port] <URL>\n", argv[0]);
    exit(-1);
  }

  proxy_host = proxy_user = proxy_pw = host = path = NULL;
  ind = 1;
  if(!strcmp(argv[ind], "-p")){
    if(parse_proxy_param(argv[++ind], &proxy_host, &proxy_port,
      &proxy_user, &proxy_pw)){
      printf("Malformed proxy parameters: '%s'", argv[2]);
      exit(EXIT_FAILURE);
    }
    ind++;
  }

  if(parse_url(argv[ind], &host, &path)){
    printf("can not parse host name and path from %s\n", argv[1]);
    exit(-1);
  }
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  if(proxy_host){
    printf("Got proxy_host:[%s], proxy_port:[%d], username:[%s], password:[%s]\n", 
          proxy_host?proxy_host:"NULL", proxy_port?proxy_port:0, 
          proxy_user?proxy_user:"NULL", proxy_pw?proxy_pw:"NULL");

    printf("connecting to %s\n", proxy_host);
    if(proxy_port)
      port_str = str_itoa(proxy_port);
    else
      port_str = str_itoa(HTTP_PORT);

    /*gethostbyname is deprecated, use getaddrinfo instead*/
    errn = getaddrinfo(proxy_host, port_str, &hints, &result);
    if(errn != 0){
      printf("getaddrinfo: %s\n", gai_strerror(errn));
      exit(EXIT_FAILURE);
    }
  } else {
    printf("connecting to %s/%s\n", host, path);
    port_str = str_itoa(HTTP_PORT);
    /*gethostbyname is deprecated, use getaddrinfo instead*/
    errn = getaddrinfo(host, port_str, &hints, &result);
    if(errn != 0){
      printf("getaddrinfo: %s\n", gai_strerror(errn));
      exit(EXIT_FAILURE);
    }
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

  printf("Retrieving document: '%s/%s'\n", host, path);
  if(!send_http_get(fd, path, host, proxy_host, proxy_user, proxy_pw))
    printf("Send http GET success\n");
  display_result(fd);
  
  if(port_str)
    free(port_str);
  close(fd);
  return 0;
}
