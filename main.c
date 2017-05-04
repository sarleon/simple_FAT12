#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

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

char buf[DISK_SIZE];

void my_print(char* str,int color);
void print_path(char* str);
void read_boot_sector();
int main(){

    



  // test print function
  char s[]="moriturus te saluto\n";
  my_print(s,0);
  char s2[]="heil caesar\n";
  my_print(s2,1);

  // main part: open the disk file

  
  FILE* f=fopen("a.img","rb");
  fread(buf,sizeof(char),DISK_SIZE,f);

  // read boot sector
  memcpy(&bpb,&buf,sizeof(bpb));
  printf("%d\n",bpb.sector_per_track); 
  printf("%d\n",bpb.bytes_per_sector); 
  printf("%d\n",bpb.root_entry_count);
  
  
  
  // read FAT
  

  // read root dir
  int root_dir_head = 19 * 512;//byte
  int entry_size = 32; //byte
  int root_dir_size = 14 * 512;
  int entry_count = root_dir_size/entry_size;
  int current_entry_head=root_dir_head;
  char* current_entry_pointer=buf+9728;
  
  struct root_entry entry;
  for(int i=0;i<entry_count ; i++){


      if(buf[current_entry_head]==0){
          current_entry_head+=entry_size;
          current_entry_pointer+=32;
          continue;
      }

      memcpy(&entry ,current_entry_pointer ,entry_size);


      printf("%s\n",entry.filename);
      printf("%s\n",entry.extension);
      
      current_entry_head+=entry_size;
      current_entry_pointer+=(32/4);
  }
  
  // user input
  char input[30];
  while(1){
    scanf("%s",&input);
    print_path(input);

    
  }


  return 0;

}


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

void read_boot_sector(){

}
