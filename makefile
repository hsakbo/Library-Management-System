build:
	cp -rf indextest data/index
	cp -rf authDataTest data/authData
	gcc -g -Wall -fPIC source/myLib.c source/deletion.c userhandle.c -o exe
	./exe

gdb:
	cp -rf indextest data/index
	cp -rf authDataTest data/authData
	gcc -g -Wall -fPIC source/myLib.c source/deletion.c userhandle.c -o exe
