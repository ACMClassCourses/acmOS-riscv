putc:
	riscv64-linux-gnu-gcc -c -o putc.o putc.c -nostdlib
	riscv64-linux-gnu-ld -z max-page-size=4096 -N -e main -Ttext 0 -o putc putc.o
	rm putc.o

clean:
	rm putc putc.o syscall.o