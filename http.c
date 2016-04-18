#include "common.h"
#include "base64/base64.h"

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
int send_http_get(int fd, const char *path, const char *host, const char *proxy_host, 
  const char *proxy_user, const char *proxy_pw){
  char command[HTTP_COMMAND_SIZE] = { 0 };

  if(proxy_host)
    snprintf(command, sizeof(command), "GET http://%s/%s HTTP/1.0\r\n", host, path);
  else
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

  /*send base64 encoded username and password*/
  if(proxy_user){
    int cred_len = strlen(proxy_user) + strlen(proxy_pw) + 1;
    unsigned char *proxy_cred = calloc(cred_len, sizeof(char));
    unsigned char *auth_str = calloc((cred_len*4/3) + 1, sizeof(char));

    snprintf((char *)proxy_cred, cred_len, "%s:%s", proxy_user, proxy_pw);
    base64_encode((const unsigned char *)proxy_cred, cred_len, auth_str);
    memset(command, 0, sizeof(command));
    snprintf(command, sizeof(command), "Proxy-Authorization: BASIC %s\r\n", auth_str);
    if(send(fd, command, strlen(command), 0) == -1){
      free(proxy_cred);
      free(auth_str);
      return -1;
    }
    free(proxy_cred);
    free(auth_str);
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

/* parse proxy parameters from spec
 * */
int parse_proxy_param(char *spec, char **proxy_host,
                      int *proxy_port, char **proxy_user, char **proxy_pw){
  char *login_sep, *colon_sep;
  printf("Proxy spec is %s\n", spec);
  if(!strncmp(spec, "http://", 7))
    spec += 7;
  login_sep = strchr(spec, '@');
  if(login_sep){  /*there is username:password */
    colon_sep = strchr(spec, ':');
    if(!colon_sep || colon_sep > login_sep){
      printf("No password in '%s'\n", spec);
      return -1;
    }
    *colon_sep = '\0';
    *proxy_user = spec;
    *proxy_pw = colon_sep + 1;
    *login_sep = '\0';
    spec = login_sep + 1;
  }

  /*proxy_server:proxy_port*/
  colon_sep = strchr(spec, ':');
  if(colon_sep){
    *colon_sep = '\0';
    *proxy_host = spec;
    *proxy_port = atoi(colon_sep + 1);
    if(*proxy_port == 0){
      printf("Wrong proxy port\n");
      exit(EXIT_FAILURE);
    }
  }else{
    *proxy_port = HTTP_PORT;
    *proxy_host = spec;
  }
  return 0;
}

