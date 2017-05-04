#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<ctype.h>

#define DISK_SIZE 1024*1440

#pragma pack (1) /*指定按1字节对齐*/

//define integer length and type
typedef unsigned char uint_8;   //1字节
typedef unsigned short uint_16; //2字节
typedef unsigned int uint_32;   //4字节
typedef unsigned long uint_64;  //8字节



struct BPB{
    char ignore[11];
    uint_16  bytes_per_sector;    //每扇区字节数
    uint_8   sector_per_cluster;    //每簇扇区数
    uint_16  reserved;    //Boot记录占用的扇区数
    uint_8   fat_numbers;   //FAT表个数
    uint_16  root_entry_count;    //根目录最大文件数
    uint_16  toal_sector_count;
    uint_8   ignore_again;
    uint_16  sector_number;   //FAT扇区数
    uint_16  sector_per_track;
    uint_16  number_of_head;
    uint_32  ignore_this;
    uint_32  toal_sector_count_fat32;  //如果BPB_FATSz16为0，该值为FAT扇区数
    }bpb;

struct root_entry{
    char filename[8];
    char extension[3];
    uint_8 attributes;
    uint_16 reserved;
    uint_16 creation_time;
    uint_16 creation_date;
    uint_16 last_access_date;
    uint_16 ignore_in_fat12;
    uint_16 last_write_date;
    uint_16 last_write_time;
    uint_16 first_logic_cluster;
    uint_32 file_size;
    
};

struct virtual_dir{
    char dir;
    char name[13];
    struct virtual_dir* childrens[64];   
};

struct virtual_file{
    char dir;
    char name[13];
    char content[512];
};



char buf[DISK_SIZE];

int find_LF_index(char* s,int l);
void recursive_find_file(struct virtual_dir* current_dir,struct root_entry *curent_entry,int sector_head);
uint_32 find_cluster_head_offset(int cluster_number);
void my_print(char* str,int color);
void print_path(char* str);
void print_dir(struct virtual_dir* dir,char* prefix);
void read_boot_sector();
int main(){

    



  // test print function
    

  // main part: open the disk file

  
  FILE* f=fopen("a.img","rb");
  fread(buf,sizeof(char),DISK_SIZE,f);

  // read boot sector
  memcpy(&bpb,&buf,sizeof(bpb));
  
  
  
  
  // read FAT
  

  // read root dir
  int root_dir_head = 19 * 512;//byte
  int entry_size = 32; //byte
  int root_dir_size = 14 * 512;
  int entry_count = root_dir_size/entry_size;
  int current_entry_head=root_dir_head;
  char* current_entry_pointer=buf+9728; 
  char namebuf[12];
  struct virtual_dir *root = malloc(sizeof(struct virtual_dir));  
  root->dir=1;
  strcpy(root->name,"root");
  struct root_entry *entry=malloc(sizeof(struct root_entry));
  for(int i=0;i<entry_count ; i++){


      if(!isalnum(buf[current_entry_head]) || islower(buf[current_entry_head]) || buf[current_entry_head]=='A'){
          current_entry_head+=entry_size;
          current_entry_pointer+=32;
          continue;
      }

      
      memcpy(entry ,current_entry_pointer ,entry_size);      
      memcpy(namebuf,entry->filename,8);
      int index= find_LF_index(namebuf, 8);
      if(entry->attributes & (uint_8)0x10){
          namebuf[index]=0;
      } else{
          namebuf[index]='.';
          memcpy(namebuf+index+1,entry->extension,3);
          namebuf[index+4]=0;
      }
          
      


      //printf("%x",find_cluster_head_offset(entry->first_logic_cluster));
      //printf("%s\n",namebuf);
      uint_8 is_dir = entry->attributes & (uint_8)0x10;

      
      if(is_dir){
          my_print("is dir",1);
          struct virtual_dir *dir = malloc(sizeof(struct virtual_dir));
          dir->dir=1;
          root->childrens[i]=dir;

          memcpy(dir->name,namebuf,13);
          recursive_find_file(dir,entry,find_cluster_head_offset(entry->first_logic_cluster));
          
      }else {
          my_print("is file",1);
          struct virtual_file *file = malloc(sizeof(struct virtual_file));
          file->dir=0;
          root->childrens[i]=file;
          memcpy(file->name,namebuf,13);
          memcpy(file->content,&buf[find_cluster_head_offset(entry->first_logic_cluster)],512);
          
          
      }
      
      current_entry_head+=entry_size;
      current_entry_pointer+=32;
  }  

  
  print_dir(root,"");
  
  // user input
  char input[30];
  while(1){
    scanf("%s",&input);
    print_path(input);

    
  }

  return 0;
  };




//test com plete
void print_path(char* str){
  char *e;
  int index;

  e = strrchr(str, '/');
  if(e==NULL){
    my_print(str,0);
    my_print("\n",0);
  } else{
    index = (int)(e - str);
    char buf[12];
    strcpy(buf,e);
    e[1]=0;
    my_print(str,0);
    my_print(buf+1,1);
    my_print("\n",1);
  }

}
uint_32 find_cluster_head_offset(int cluster_number){
    int sector_number = cluster_number+33 - 2;
    int sector_offset = 512 * sector_number;
    return sector_offset;
    
}
void read_boot_sector(){

}

void recursive_find_file(struct virtual_dir* current_dir,struct root_entry* cuurent_entry,int sector_head){
  
  int root_dir_head = 19 * 512;//byte
  int entry_size = 32; //byte
  int root_dir_size = 14 * 512;
  int entry_count = root_dir_size/entry_size;
  int current_entry_head=sector_head;    

  char namebuf[12];

  char* current_entry_pointer=buf+sector_head;
  struct virtual_dir *root = calloc(sizeof(struct virtual_dir),1);  
  struct root_entry *entry=calloc(sizeof(struct root_entry),1);
  for(int i=0;i<entry_count ; i++){

       
      if(!isalnum(buf[current_entry_head]) || islower(buf[current_entry_head]) || buf[current_entry_head]=='A'){
          current_entry_head+=entry_size;
          current_entry_pointer+=32;
          continue;
      }
      
      memcpy(entry ,current_entry_pointer ,entry_size);      
      memcpy(namebuf,entry->filename,8);
      int index= find_LF_index(namebuf, 8);
      
      if(entry->attributes & (uint_8)0x10){
          namebuf[index]=0;
      } else{
          namebuf[index]='.';
          memcpy(namebuf+index+1,entry->extension,3);
          namebuf[index+4]=0;
      }
      
      
      
      //printf("%s\n",namebuf);
      uint_8 is_dir = entry->attributes & (uint_8)0x10;
      
      
      if(is_dir){
          
          struct virtual_dir *dir = calloc(sizeof(struct virtual_dir),1);
          dir->dir=1;
          current_dir->childrens[i]=dir;
          memcpy(dir->name,namebuf,13);

          recursive_find_file(dir,entry,find_cluster_head_offset(entry->first_logic_cluster));

      } else {
          
          struct virtual_file *file = calloc(sizeof(struct virtual_file),1);
          file->dir=0;
          current_dir->childrens[i]=file;
          memcpy(file->name,namebuf,13);
          memcpy(file->content,&buf[find_cluster_head_offset(entry->first_logic_cluster)],512);          
      }
      
      current_entry_head+=entry_size;
      current_entry_pointer+=32;
  }
}

int find_LF_index(char* s,int l){
    for(int i=0 ; i<l ; i++){
        if(*(s+i)=='\n' || *(s+i)=='\x20'){
            return i;
        }
    }
    return l-1;
}

void print_dir(struct virtual_dir* dir,char *prefix){
    my_print("invoking:",1);
    my_print(dir->name,1);
    char *cur_prefix = calloc(100,1);
    char *cur_filename= calloc(100,1);
    char *slash = calloc(2,1);
    slash[0]='/';
    slash[1]=0;

    strcat(cur_prefix,dir->name);
    strcat(cur_prefix,slash);
    for(int i=0 ; i<64 ;i++){
 

        if(dir->childrens[i]==NULL){
            continue;
        }

       
        if(dir->childrens[i]->dir){
            my_print("dir:\n",1);
            print_dir(dir->childrens[i], cur_prefix);
        } else {
            my_print("file:\n",1);
            strcpy(cur_filename,cur_prefix);
            strcat(cur_filename,dir->childrens[i]->name);
            print_path(cur_filename);
        }
        
        if(dir->dir!=0){
            //file
   
    
        } else {

        }

    }
    
}
