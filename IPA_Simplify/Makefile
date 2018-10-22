include ../Makefile.rules


IFLAGS = $(BASEINCPATH) \
	 	 $(OPENCVINCPATH) \
		 $(EIGENINCPATH) \
	     $(EMBREEINCPATH)\
	     $(OPENMESHINCPATH) \
	     $(BOOSTINCPATH)

LFLAGS =  $(BASELIBPATH)   -lprsori -lmeshutil \
	  $(EMBREELIBPATH) \
	  $(OPENCVLIBPATH) $(LIBOCVHIGHGUI) $(LIBOCVCALIB3D) $(LIBOCVIMGPROC) $(LIBOCOBJDETECT)  $(LIBOCVCORE) $(LIBOCVEXTRA)\
	  $(EMBREELIBPATH) \
	  $(OPENMESHLIBPATH) \
	  $(BOOSTFILESYSTEMLIBPATHSTAT)


OBJECTS =  

all: lib exe
lib: $(OBJECTS)
exe: IPA_Simplify.o test_simplify

.PHONY:  all


IPA_Simplify.o: IPA_Simplify.cpp
	@echo ${cc_green} Compile IPA_Simplify.cpp ...  ${cc_nocolor}
	${CC} ${CFLAGS} -c IPA_Simplify.cpp ${IFLAGS}
	@echo ${cc_green}... DONE${cc_nocolor}

test_simplify: IPA_Simplify.o ${OBJECTS}
	@echo ${cc_green}Link IPA_Simplify...${cc_nocolor}
	${CC} ${CCFLAGS} -o test_simplify IPA_Simplify.o ${OBJECTS} ${LFLAGS}
	#@mv test_ori ./tests
	@echo ${cc_green}...Done ${cc_nocolor}

libprsori.a: ${OBJECTS}
	@echo ${cc_green} Archive...${cc_nocolor}
	ar rcs libprsori.a ${OBJECTS}
	@mv libprsori.a ${LIBPATH}
	@echo ${cc_green}...Done${cc_nocolor}


.PHONY: clean
clean:
	@rm -rvf *.o
