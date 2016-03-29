#include "common.h"

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

/*send http get commands
 * */
int send_http_get(int fd, const char *path, const char *host){
  char command[HTTP_COMMAND_SIZE] = { 0 };

  snprintf(command, sizeof(command), "GET /%s HTTP/1.0\r\n", path);
  if(send(fd, command, strlen(command), 0) == -1){
    printf("Error when send comamnd:\n%s\n", command);
    return -1;  
  }

  memset(command, 0, sizeof(command));
  snprintf(command, sizeof(command), "Host: %s\r\n", host);
  if(send(fd, command, strlen(command), 0) == -1){
    printf("Error when send comamnd:\n%s\n", command);
    return -1;  
  }

  memset(command, 0, sizeof(command));
  snprintf(command, sizeof(command), "Connection: close\r\n\r\n");
  if(send(fd, command, strlen(command), 0) == -1){
    printf("Error when send comamnd:\n%s\n", command);
    return -1;  
  }

  return 0;
}

/*
 *Read and display get result
 * */
int display_result(int fd){
  int len;
  char buf[BUF_SIZE + 1] = { 0 };

  while((len = recv(fd, buf, BUF_SIZE, 0)) > 0){
    buf[len] = '\0';
    printf("%s\n", buf);
  }
  return 0;
}
