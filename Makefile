client : client.cc
	g++ -I include `pkg-config --cflags --libs poppler` client.cc /usr/lib/libpoppler.so -o pdfextractor
match.o : match.c
	#gcc -I include lib/libfeat.a `pkg-config --cflags --libs opencv ` `pkg-config --cflags --libs poppler` /usr/lib/libstdc++.so.6 /usr/lib/libopenjpeg.so client.cc -o sticazzi
	cc -I include `pkg-config --cflags --libs opencv `  match.c  /usr/lib/libopenjpeg.so /lib/libm.so.6 lib/libfeat.a -o match.o 

clean :
	rm *.o

##PER UBUNTU VA AGGIUNTO IL LINK SIMBOLICO A libm-2.13.so in /lib/libm.so.6
##PER UBUNTU 11.10 L'OPZIONE DI COMPILAZIONE DEL MAIN DI MATCH e':	cc -I include `pkg-config --cflags --libs opencv `  match.c -o match.o -L lib -lfeat `pkg-config --libs opencv ` 
##PER UBUNTU 10.04 e' 	cc -I include `pkg-config --cflags --libs opencv `  match.c  /usr/lib/libopenjpeg.so /lib/libm.so.6 lib/libfeat.a -o match.o 

