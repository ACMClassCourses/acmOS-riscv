#include "stdlib.h"



int main(){
    print("Hello world!\n");
    sys_exit(0);
    print("putc: Should never be here!");
    while(1) ;
}