client : client.cc
	g++ -I include `pkg-config --cflags --libs poppler` client.cc /usr/lib/libpoppler.so -o sticazzi
#match.o : match.c
	#gcc -I include lib/libfeat.a `pkg-config --cflags --libs opencv ` `pkg-config --cflags --libs poppler` /usr/lib/libstdc++.so.6 /usr/lib/libopenjpeg.so client.cc -o sticazzi
	#cc -I include `pkg-config --cflags --libs opencv `  match.c /usr/lib/libopenjpeg.so /lib/libm.so.6 lib/libfeat.a -o match.o 

clean :
	rm *.o
