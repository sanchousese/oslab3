#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h> 
#include <string.h>

void fileinfo2string(char*);
void read_entity(char*);

int main(int argc, char** argv)
{
  printf("Filename  Accesses  NHL  Groupname  Username  Last Mod Time\n");

  for(int i = 1; i < argc; ++i)
  {
    read_entity(argv[i]);
  }

  return 0;
}

void fileinfo2string(char* filename)
{
  char* result = (char*)malloc(sizeof(char) * 100);
  struct stat info;
  stat(filename, &info);

  struct passwd *pw = getpwuid(info.st_uid);
  struct group  *gr = getgrgid(info.st_gid);
  int accesses = info.st_mode & 000777;

  printf("%s  %o  %d  %s  %s  %s", filename, accesses, info.st_nlink, pw->pw_name, gr->gr_name, ctime(&info.st_ctime));

  return result;
}

void read_entity(char* name)
{
  DIR           *d;
  struct dirent *dir;
  d = opendir(name);

  if(d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      if(strcmp(".", dir->d_name) != 0 && strcmp("..", dir->d_name) != 0)
      {
        char* result = (char*)malloc(sizeof(char) * 200);

        strcpy(result, name);
        strcat(result, "/");
        strcat(result, dir->d_name);

        read_entity(result);
      }
    }

    closedir(d);
  }
  else
  {
    fileinfo2string(name);
  }
}