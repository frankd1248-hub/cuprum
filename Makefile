
make:
	g++ -O3 -march=x86-64-v2 -mtune=generic -fno-plt -flto=3 -fvisibility=hidden \
		-o dist/cuprum src/*.cpp -std=c++23

lib:
	gcc -O3 -march=x86-64-v2 -mtune=generic -fno-stack-protector -c ./stdlib/*.c -o ./stdlib/std.o