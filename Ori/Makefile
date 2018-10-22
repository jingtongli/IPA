include ../Makefile.rules


IFLAGS = $(BASEINCPATH) \
	 	 $(OPENCVINCPATH) \
		 $(EIGENINCPATH) \
	     $(EMBREEINCPATH)\
	     $(OPENMESHINCPATH) \
	     $(BOOSTINCPATH)

LFLAGS =  $(BASELIBPATH)   \
	  $(EMBREELIBPATH) \
	  $(OPENCVLIBPATH) $(LIBOCVHIGHGUI) $(LIBOCVCALIB3D) $(LIBOCVIMGPROC) $(LIBOCOBJDETECT)  $(LIBOCVCORE) $(LIBOCVEXTRA)\
	  $(EMBREELIBPATH) \
	  $(OPENMESHLIBPATH) \
	$(BOOSTFILESYSTEMLIBPATHSTAT)


OBJECTS = GeomUtil.o Orientation.o OriGeom.o OriListIO.o

all: lib exe
lib: $(OBJECTS) test_ori.o test_orilistio.o libprsori.a
exe: test_ori test_orilistio

.PHONY:  all

GeomUtil.o: GeomUtil.h GeomUtil.cpp
	@echo ${cc_green} Compile GeomUtil.cpp ...  ${cc_nocolor}
	${CC} ${CFLAGS} -c GeomUtil.cpp ${IFLAGS}
	@echo ${cc_green}... DONE${cc_nocolor}

OriGeom.o: OriGeom.h OriGeom.cpp
	@echo ${cc_green} Compile OriGeom.cpp ...  ${cc_nocolor}
	${CC} ${CFLAGS} -c OriGeom.cpp ${IFLAGS}
	@echo ${cc_green}... DONE${cc_nocolor}

Orientation.o: Orientation.h Orientation.cpp
	@echo ${cc_green} Compile Orientation.cpp ...  ${cc_nocolor}
	${CC} ${CFLAGS} -c Orientation.cpp ${IFLAGS}
	@echo ${cc_green}... DONE${cc_nocolor}

OriListIO.o: OriListIO.h OriListIO.cpp
	@echo ${cc_green} Compile OriListIO.cpp ...  ${cc_nocolor}
	${CC} ${CFLAGS} -c OriListIO.cpp ${IFLAGS}
	@echo ${cc_green}... DONE${cc_nocolor}

test_ori.o:  test_ori.cpp
	@echo ${cc_green} Compile test_ori.cpp ...  ${cc_nocolor}
	${CC} ${CFLAGS} -c test_ori.cpp ${IFLAGS}
	@echo ${cc_green}... DONE${cc_nocolor}

test_orilistio.o:  test_orilistio.cpp
	@echo ${cc_green} Compile test_orilistio.cpp ...  ${cc_nocolor}
	${CC} ${CFLAGS} -c test_orilistio.cpp ${IFLAGS}
	@echo ${cc_green}... DONE${cc_nocolor}

test_ori: test_ori.o ${OBJECTS}
	@echo ${cc_green}Link test_ori...${cc_nocolor}
	${CC} ${CCFLAGS} -o test_ori test_ori.o ${OBJECTS} ${LFLAGS}
	#@mv test_ori ./tests
	@echo ${cc_green}...Done ${cc_nocolor}

test_orilistio: test_orilistio.o ${OBJECTS}
	@echo ${cc_green}Link test_orilistio...${cc_nocolor}
	${CC} ${CCFLAGS} -o test_orilistio test_orilistio.o ${OBJECTS} ${LFLAGS}
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


