match : match.c
	#g++ -I ../ `pkg-config --cflags --libs poppler` /usr/lib/libopenjpeg.so pdftotext.cc -o sticazzi
	cc -I include `pkg-config --cflags --libs opencv ` match.c /lib/libm.so.6 lib/libfeat.a -o match 
touch_it:
	touch match.c
clean :
	rm *.o
