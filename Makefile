POPPLER_LIB = `pkg-config --cflags --libs poppler`
OPENCV_LIB = `pkg-config --cflags --libs opencv `
SQLITE_LIB = `pkg-config --cflags --libs sqlite3 `
SRC_DIR = src
FEAT_LIB = lib/libfeat.a 
JHEAD_LIB = lib/libhead.a

OBJS= $(SRC_DIR)/match.o $(SRC_DIR)/pdftotext.o $(SRC_DIR)/sift_template.o $(SRC_DIR)/template_extractor.o $(SRC_DIR)/database.o
EXECUTABLE = pdfextractor

all : libhead libfeat objs
	g++ client.cc -I include -w $(OBJS) $(SQLITE_LIB) $(POPPLER_LIB) $(OPENCV_LIB) $(JHEAD_LIB) $(FEAT_LIB) -o $(EXECUTABLE)  

objs: libfeat libhead
	make -C $(SRC_DIR)

libfeat : 
	make -C libfeat-src

libhead:
	make -C libjhead-src/

install :
	mv pdfextractor /usr/bin

uninstall :
	rm /usr/bin/pdfextractor 



clean :
	rm libfeat-src/*.o
	rm libjhead-src/*.o
	rm lib/libfeat.a
	rm lib/libhead.a
	rm -f note*
	rm $(EXECUTABLE)
	make -C $(SRC_DIR)/ clean


