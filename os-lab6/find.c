#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void
find(char *path, char *target, int show_inode)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(st.type != T_DIR){
    close(fd);
    return;
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
    fprintf(2, "find: path too long\n");
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;

    char name[DIRSIZ + 1];
    memmove(name, de.name, DIRSIZ);
    name[DIRSIZ] = 0;

    // Skip . and .. to prevent infinite recursion
    if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
      continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    // Trim trailing spaces if any
    int len = strlen(p);
    while(len > 0 && p[len - 1] == ' ') {
      p[len - 1] = '\0';
      len--;
    }

    if(stat(buf, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", buf);
      continue;
    }

    if(strcmp(name, target) == 0){
      if(show_inode)
        printf("%s %d\n", buf, st.ino);
      else
        printf("%s\n", buf);
    }

    if(st.type == T_DIR){
      find(buf, target, show_inode);
    }
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  int show_inode = 0;

  if(argc < 3){
    fprintf(2, "usage: find <path> <name> [-n]\n");
    exit(1);
  }

  if(argc >= 4 && strcmp(argv[3], "-n") == 0)
    show_inode = 1;

  find(argv[1], argv[2], show_inode);
  exit(0);
}
