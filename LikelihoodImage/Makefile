include ../Makefile.rules


IFLAGS = $(BASEINCPATH) \
	 $(OPENCVINCPATH) \
	 $(EIGENINCPATH) \
	 $(EMBREEINCPATH)\
	 $(OPENMESHINCPATH) \
	$(BOOSTINCPATH)

LFLAGS =  $(BASELIBPATH) -liolist -lprstimer -lmeshutil \
	  $(EMBREELIBPATH) \
	  $(OPENCVLIBPATH) $(LIBOCVHIGHGUI) $(LIBOCVCALIB3D) $(LIBOCVIMGPROC) $(LIBOCOBJDETECT)  $(LIBOCVCORE) $(LIBOCVEXTRA)\
	  $(EMBREELIBPATH) \
	  $(OPENMESHLIBPATH) \
	$(BOOSTFILESYSTEMLIBPATHSTAT)


OBJECTS = LikelihoodImage.o

all: lib exe
lib: $(OBJECTS)  liblikelihoodimage.a
exe: 

.PHONY:  all

LikelihoodImage.o: LikelihoodImage.h LikelihoodImage.cpp
	@echo ${cc_green} Compile LikelihoodImage.cpp ... ${cc_nocolor}
	${CC} ${CFLAGS} -c LikelihoodImage.cpp ${IFLAGS}
	@echo ${cc_green}... DONE${cc_nocolor}

test_likelihoodimage.o:  test_likelihoodimage.cpp
	@echo ${cc_green} Compile test_likelihoodimage.cpp ...  ${cc_nocolor}
	${CC} ${CFLAGS} -c test_likelihoodimage.cpp ${IFLAGS}
	@echo ${cc_green}... DONE${cc_nocolor}

test_likelihoodimage: test_likelihoodimage.o ${OBJECTS}
	@echo ${cc_green}Link test_likelihoodimage...${cc_nocolor}
	${CC} ${CCFLAGS} -o test_likelihoodimage test_likelihoodimage.o ${OBJECTS} ${LFLAGS}
	#@mv test_iolist ./tests
	@echo ${cc_green}...Done ${cc_nocolor}

liblikelihoodimage.a : ${OBJECTS}
	@echo ${cc_green} Archive...${cc_nocolor}
	ar rcs liblikelihoodimage.a ${OBJECTS}
	@mv liblikelihoodimage.a ${LIBPATH}
	@echo ${cc_green}...Done${cc_nocolor}


.PHONY: clean
clean:
	@rm -rvf *.o


