POPPLER_LIB = `pkg-config --cflags --libs poppler`
ROOT_DIR = ..
INCLUDE_DIR = $(ROOT_DIR)/include

OBJ = sift_template.o template_extractor.o


all : match.c database.c pdftotext.cpp objs
	cc -c database.c -I $(INCLUDE_DIR) -w 
	g++ -c pdftotext.cpp -I $(INCLUDE_DIR) -w $(POPPLER_LIB) 
	cc -c match.c -I $(INCLUDE_DIR) -DDEBUG -w 
	
objs: sift_template.c template_extractor.c
	cc -c sift_template.c -I $(INCLUDE_DIR) -w  
	cc -c template_extractor.c -I $(INCLUDE_DIR) -w 

clean :
	rm *.o

