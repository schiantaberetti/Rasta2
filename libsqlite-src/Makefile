#--------------------------------
# jhead makefile for Unix
#--------------------------------
OBJ=.
SRC=.
CFLAGS= -O3 -Wall
FILE_OBJ= sqlite3.o
OBJS= $(OBJ)/sqlite3.o
LIB_DIR	= ../lib
INCLUDE= ../include

all: libsqlite.a

libsqlite.a: $(OBJS)
	ar rc $(LIB_DIR)/$@ $(FILE_OBJ)
	ranlib $(LIB_DIR)/$@

$(OBJ)/%.o:$(SRC)/%.c
	${CC} -I $(INCLUDE) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(FILE_OBJ) libsqlite.a

#jhead: $(objs) jhead.h
#	${CC} -o jhead_global.o $(objs) -lm

#clean:
#	rm -f $(objs) 

#install:
#	cp jhead ${DESTDIR}/usr/local/bin/
