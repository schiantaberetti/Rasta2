POPPLER_LIB = `pkg-config --cflags --libs poppler`
OPENCV_LIB = `pkg-config --cflags --libs opencv `
FEAT_LIB = lib/libfeat.a lib/libhead.a

EXECUTABLE = pdfextractor

all : libhead libfeat client.cc match.o sqlite3.o
	g++ client.cc -I include -w match.o $(POPPLER_LIB) $(OPENCV_LIB) $(FEAT_LIB) -o $(EXECUTABLE)  

match.o : match.c
	cc -c match.c -I include -w $(OPENCV_LIB) $(FEAT_LIB) 

sqlite3.o: 
	cc -c sqlite3.c -I include -w  
	
libfeat : 
	cd libfeat-src/
	make -C libfeat-src

libhead:
	cd jhead-src
	make -C jhead-src/

install :
	mv pdfextractor /usr/bin

uninstall :
	rm /usr/bin/pdfextractor 



clean :
	rm libfeat-src/*.o
	rm jhead-src/*.o
	rm lib/libfeat.a
	rm lib/libhead.a
	rm *.o
	rm -f note*
	rm $(EXECUTABLE)


##PER UBUNTU VA AGGIUNTO IL LINK SIMBOLICO A libm-2.13.so in /lib/libm.so.6
##PER UBUNTU 11.10 L'OPZIONE DI COMPILAZIONE DEL MAIN DI MATCH e':	cc -I include `pkg-config --cflags --libs opencv `  match.c -o match.o -L lib -lfeat `pkg-config --libs opencv ` 
##PER UBUNTU 10.04 e' 	cc -I include `pkg-config --cflags --libs opencv `  match.c  /usr/lib/libopenjpeg.so /lib/libm.so.6 lib/libfeat.a -o match.o 

