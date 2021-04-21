if [ ! -d "build" ]; then
    mkdir build
fi

echo "compiling kernel ..."
cd build

cmake -DCMAKE_LINKER=riscv64-linux-gnu-ld -DCMAKE_C_COMPILER=riscv64-linux-gnu-gcc -DCMAKE_ASM_COMPILER=riscv64-linux-gnu-gcc -DCMAKE_C_LINK_EXECUTABLE="<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>" .. -G "Unix Makefiles" "$@"

echo "before make"
make
echo "after make"
riscv64-linux-gnu-nm -n kernel.img > kernel.sym