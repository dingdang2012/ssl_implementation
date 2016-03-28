/*parse hostname and path from url
 * return 0 for success
 * -1 for fail
 * */
int parse_url(char *url, char **host, char **path){
  char *p = strstr(url, "//");
  if(!p)
    return -1;

  *host = p + 2;
  p = strchr(host, '/');
  if(!p)
    *path = NULL;
  else{
    *path = p + 1;
    *p = '\0';
  }
  return 0;
}
