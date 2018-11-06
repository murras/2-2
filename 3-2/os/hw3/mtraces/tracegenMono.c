#include <stdlib.h>
#include <stdio.h>

#define MEMSIZEBITS 32

int main(int argc, char *argv[]) {
    int i, addr, traceSize;

    if (argc < 3) {
	printf("Usage : %s mono_addr_value tracesize\n",argv[0]); exit(1);
    }

    addr = atoi(argv[1]);
    traceSize = atoi(argv[2]);

//    printf("%d %d\n",addr,traceSize);
    
    for(i = 0; i < traceSize; i++) {
       if (addr % 2 == 0) {
	  printf("%08x W\n",addr %(1<<MEMSIZEBITS-1));
       }
       else {
	  printf("%08x R\n",addr %(1<<MEMSIZEBITS-1));
       }
    }
}
