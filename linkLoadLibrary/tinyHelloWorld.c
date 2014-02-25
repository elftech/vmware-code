//Compilation 
//gcc -c -fno-builtin tinyHelloWorld.c
//ld -static -e nomain -o tinyHelloWorld tinyHelloWorld.o
//This program doesn't work
char *str="Hello World!\n";

void print()
{
	asm("movl $13,%%edx \n\t"
	    "movl %0,%%ecx \n\t"
	    "movl $0,%%edx \n\t"
	    "movl $4,%%eax \n\t"
	    "int $0x80     \n\t"
	    ::"r"(str):"edx","ecx","ebx");
}

void exit()
{
	asm("movl $42,%ebx \n\t"
            "movl $1,%eax \n\t"
	    "int $0x80    \n\t");
}

void nomain()
{
	print();
	exit();
}
