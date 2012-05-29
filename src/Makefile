POPPLER_LIB = `pkg-config --cflags --libs poppler`
OPENCV_LIB = `pkg-config --cflags --libs opencv `
SQLITE_LIB = `pkg-config --cflags --libs sqlite3 `
ROOT_DIR = ..
FEAT_LIB = $(ROOT_DIR)/lib/libfeat.a 
JHEAD_LIB = $(ROOT_DIR)/lib/libhead.a
INCLUDE_DIR = $(ROOT_DIR)/include

OBJ = sift_template.o template_extractor.o


all : match.c database.c pdftotext.cpp objs
	cc -c database.c -I $(INCLUDE_DIR) -w $(SQLITE_LIB)
	g++ -c pdftotext.cpp -I $(INCLUDE_DIR) -w $(POPPLER_LIB) 
	cc -c match.c -I $(INCLUDE_DIR) -w $(OBJ) $(OPENCV_LIB) $(FEAT_LIB) $(JHEAD_LIB)	

objs: sift_template.c template_extractor.c
	cc -c sift_template.c -I $(INCLUDE_DIR) -w $(OPENCV_LIB) $(FEAT_LIB)
	cc -c template_extractor.c -I $(INCLUDE_DIR) -w $(OPENCV_LIB)

clean :
	rm *.o

