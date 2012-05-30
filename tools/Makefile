ROOT_DIR = ..
FEAT_LIB = $(ROOT_DIR)/lib/libfeat.a 
OPENCV_LIB = `pkg-config --cflags --libs opencv `
MATH_LIB = -lm

INCLUDE_DIR = $(ROOT_DIR)/include
EXECUTABLE = img2sifts


img2sifts:
	cc img2sifts.c -I $(INCLUDE_DIR) $(OPENCV_LIB) $(FEAT_LIB) $(MATH_LIB) -o $(EXECUTABLE) 

clean :
	rm $(EXECUTABLE)

