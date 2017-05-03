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
  uint_16  BPB_BytsPerSec;    //每扇区字节数
  uint_8   BPB_SecPerClus;    //每簇扇区数
  uint_16  BPB_RsvdSecCnt;    //Boot记录占用的扇区数
  uint_8   BPB_NumFATs;   //FAT表个数
  uint_16  BPB_RootEntCnt;    //根目录最大文件数
  uint_16  BPB_TotSec16;
  uint_8   BPB_Media;
  uint_16  BPB_FATSz16;   //FAT扇区数
  uint_16  BPB_SecPerTrk;
  uint_16  BPB_NumHeads;
  uint_32  BPB_HiddSec;
  uint_32  BPB_TotSec32;  //如果BPB_FATSz16为0，该值为FAT扇区数
}bpb;


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
  fgets(buf,DISK_SIZE,f);

  // read boot sector
  memcpy(&bpb,&buf,sizeof(bpb));
  printf("%d",bpb.BPB_FATSz16);
  // handle user input
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
