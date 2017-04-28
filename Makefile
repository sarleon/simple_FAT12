my_print:my_print.asm
	nasm my_print.asm -f elf64 -o my_print.o
main: main.c  my_print.o
	gcc  main.c -c  -g -o main.o
	gcc main.o my_print.o -o fat12
	chmod a+x fat12
	rm *.o
all:my_print main
clean:
	rm fat12
