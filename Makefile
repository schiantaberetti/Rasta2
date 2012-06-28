POPPLER_LIB = `pkg-config --cflags --libs poppler`
OPENCV_LIB = `pkg-config --cflags --libs opencv `
SQLITE_LIB = `pkg-config --cflags --libs sqlite3 `
SRC_DIR = src
FEAT_LIB = lib/libfeat.a 
JHEAD_LIB = lib/libhead.a
INCLUDE_DIR = include

OBJS= $(SRC_DIR)/match.o $(SRC_DIR)/pdftotext.o $(SRC_DIR)/sift_template.o $(SRC_DIR)/template_extractor.o $(SRC_DIR)/database.o
EXECUTABLE = pdfextractor

all : libhead libfeat objs tools 
	g++ client.cc -I $(INCLUDE_DIR) -w $(OBJS)  $(SQLITE_LIB) $(POPPLER_LIB) $(OPENCV_LIB) $(JHEAD_LIB) $(FEAT_LIB) -o $(EXECUTABLE)  

tools : libfeat
	make -C tools/

objs: libfeat libhead
	make -C $(SRC_DIR)

libfeat : 
	make -C libfeat-src/

libhead:
	make -C libjhead-src/

install :
	mv pdfextractor /usr/bin

uninstall :
	rm /usr/bin/pdfextractor 



clean :
	make -C tools clean
	rm libfeat-src/*.o
	rm libjhead-src/*.o
	rm lib/libfeat.a
	rm lib/libhead.a
	rm -f note*
	rm -f script/note*
	rm -rf script/database
	rm $(EXECUTABLE)
	make -C $(SRC_DIR)/ clean


