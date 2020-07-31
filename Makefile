build:
	gcc -I network_math/network_math.h main.c network_math/endian.c network_math/ip_maths.c	

debug:
	gcc -g -I network_math/network_math.h main.c network_math/endian.c network_math/ip_maths.c
	gdb ./a.out
