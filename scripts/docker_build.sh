rm -rf ./build

docker run -it --rm -v $(pwd):$(pwd) -w $(pwd) riscv64-build /bin/bash ./scripts/build.sh