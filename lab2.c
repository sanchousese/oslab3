#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h> 
#include <string.h>

#define ENTRY_POINT_DIR_NAME_SIZE 200
#define MAX_FILENAME_SIZE 1000
#define MAX_FILE_INODES 1000
#define FILES_PERIOD_TO_PRINTF 10000

#define allocate(type, size) (type*)malloc(sizeof(type) * size)
#define fill_array(arr, size, default_value) for (int i = 0; i < size; ++i) {arr[i] = default_value;}


typedef struct file_inode
{
  int inode_id;
  int n_links;
  int n_filenames;
  char** filenames;
} file_inode;

struct inodes_collection
{
  int size;
  file_inode** file_inodes;
} inodes;

int total_files_processed = 0;


void read_fs_item(char* item_name);
void check_file(char* filename);
void init_inodes();void modify_inodes(file_inode_id, n_links, filename);
int get_inode_index(int inode_id);
void add_file_inode(int file_inode_id, int n_links, char* filename);
void update_inode(int inode_index, char* filename);
void print_inodes();


int main(int argc, char** argv)
{
  char* entry_point = allocate(char, ENTRY_POINT_DIR_NAME_SIZE);
  init_inodes();

  if (argc != 2)
  {
    printf("Error.\nUsage: ./lab3 entry_point_dir\n");
    return 1;
  }

  strcpy(entry_point, argv[1]);

  DIR           *d;
  // struct dirent *dir;
  d = opendir(entry_point);

  if(d)
  {
    read_fs_item(entry_point);
  }
  else
  {
    printf("Error. \"%s\" is not a directory.\n", entry_point);
  }

  // printf("%s\n", entry_point);

  // printf("Filename  Accesses  NHL  Groupname  Username  Last Mod Time\n");

  // for(int i = 1; i < argc; ++i)
  // {
  //   read_entity(argv[i]);
  // }

  printf("before final pring\n");
  print_inodes();

  return 0;
}

void read_fs_item(char* item_name)
{
  DIR *d;
  struct dirent *dir;
  d = opendir(item_name);

  if(d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      if(strcmp(".", dir->d_name) != 0 && strcmp("..", dir->d_name) != 0)
      {
        char* subitem_name = allocate(char, MAX_FILENAME_SIZE);

        strcpy(subitem_name, item_name);
        strcat(subitem_name, "/");
        strcat(subitem_name, dir->d_name);

       read_fs_item(subitem_name);

       free(subitem_name);
      }
    }

    closedir(d);
  }
  else
  {
    check_file(item_name);

    total_files_processed++;
    if (total_files_processed % FILES_PERIOD_TO_PRINTF == 0)
    {
      printf("Files processed: %d\n", total_files_processed);
      printf("Last processed file: %s\n", item_name);
    }
  }
}

void check_file(char* filename)
{
  struct stat info;
  stat(filename, &info);
  int file_inode_id = info.st_ino;
  int n_links = info.st_nlink;
  if (n_links > 1)
  {
    modify_inodes(file_inode_id, n_links, filename);
  }
  //printf("filename: %s   inode_id: %d   n_links: %d\n", filename, file_inode_id, n_links);

}

void modify_inodes(file_inode_id, n_links, filename)
{
  int index;
  if (index = get_inode_index(file_inode_id) == -1)
  {
    add_file_inode(file_inode_id, n_links, filename);
  }
  else
  {
    update_inode(index, filename);
  }
}

int get_inode_index(int inode_id)
{
  for (int i = 0; i < inodes.size; ++i)
  {
    if (inodes.file_inodes[i]->inode_id == inode_id)
    {
      return i;
    }
  }
  return -1;
}

void add_file_inode(int file_inode_id, int n_links, char* filename)
{
  file_inode* new_inode = allocate(file_inode, 1);
  new_inode->inode_id = file_inode_id;
  new_inode->n_links = n_links;
  new_inode->filenames = allocate(char*, n_links);
  new_inode->filenames[0] = allocate(char*, MAX_FILENAME_SIZE);
  strcpy(new_inode->filenames[0], filename);
  new_inode->n_filenames = 1;
  inodes.file_inodes[inodes.size++] = new_inode;
}

void update_inode(int inode_index, char* filename)
{
  file_inode* cur_inode = inodes.file_inodes[inode_index];
  char* filename_copy = allocate(char, MAX_FILENAME_SIZE);
  strcpy(filename_copy, filename);
  cur_inode->filenames[cur_inode->n_filenames++] = filename_copy;
}

void print_inodes()
{
  for (int i = 0; i < inodes.size; ++i)
  {
    file_inode* cur_inode = inodes.file_inodes[i];
    printf("inode #%d:\n", cur_inode->inode_id);
    printf("Hard Links amount: %d\n", cur_inode->n_links);
    printf("Filenames:\n");
    for (int j = 0; j < cur_inode->n_filenames; ++j)
    {
      printf("%s\n", cur_inode->filenames[j]);
    }
    printf("\n\n");
  }
}

void init_inodes()
{
  inodes.file_inodes = allocate(file_inode*, MAX_FILE_INODES);
  //fill_array(inodes.file_inodes, MAX_FILE_INODES, NULL);
  inodes.size = 0;
}