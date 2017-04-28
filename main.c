#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
//
#define DISK_SIZE 1024*1440



char buf[DISK_SIZE];

void my_print(char* str,int color);
void print_path(char* str);
int main(){

  // test print function
  char s[]="moriturus te saluto\n";
  my_print(s,0);
  char s2[]="heil caesar\n";
  my_print(s2,1);

  // main part: open the disk file

  FILE* f=fopen("a.img","rb");
  fgets(buf,DISK_SIZE,f);

  // handle user input
  char input[30];
  while(1){
    scanf("%s",&input);
    print_path(input);
  }


  return 0;

}


//test complete
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
    e[0]=0;
    my_print(str,0);
    my_print(buf,1);
    my_print("\n",1);
  }

}
