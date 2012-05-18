#--------------------------------
# jhead makefile for Unix
#--------------------------------
OBJ=.
SRC=.
CFLAGS= -O3 -Wall
FILE_OBJ= jhead.o jpgfile.o paths.o exif.o iptc.o gpsinfo.o makernote.o
OBJS= $(OBJ)/jhead.o $(OBJ)/jpgfile.o $(OBJ)/paths.o \
	$(OBJ)/exif.o $(OBJ)/iptc.o $(OBJ)/gpsinfo.o $(OBJ)/makernote.o
LIB_DIR	= ../lib
INCLUDE= ../include

all: libhead.a

libhead.a: $(OBJS)
	ar rc $(LIB_DIR)/$@ $(FILE_OBJ)
	ranlib $(LIB_DIR)/$@

$(OBJ)/%.o:$(SRC)/%.c
	${CC} -I $(INCLUDE) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(FILE_OBJ) libhead.a

#jhead: $(objs) jhead.h
#	${CC} -o jhead_global.o $(objs) -lm

#clean:
#	rm -f $(objs) 

#install:
#	cp jhead ${DESTDIR}/usr/local/bin/
