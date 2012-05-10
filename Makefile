match : match.c

	cc -I include `pkg-config --cflags --libs opencv ` match.c /lib/libm.so.6 lib/libfeat.a -o match 
touch_it:
	touch match.c
clean :
	rm *.o
