UNAME := $(shell uname -s)
R_HOME=/Library/Frameworks/R.framework/Versions/2.11/Resources
MONGO_HOME=mongo-cxx-driver/
run:
	g++ -o mongoR.o -c -O3 -D_SCONS -DMONGO_EXPOSE_MACROS -I ${R_HOME}/include -I${MONGO_HOME}mongo -I${MONGO_HOME}boost -I. -I${MONGO_HOME}pcre mongoR.cc
    ifeq ($(UNAME),Darwin)
        # Set the boost library location
		R CMD SHLIB *.o -o ../bin/librmongo-darwin-64.so -Llibs/darwin-64 -Bstatic -lmongoclient -lpthread -lboost_thread -lboost_filesystem -lboost_system
    else
		R CMD SHLIB *.o -o librmongo.so -L./mongo-cxx-driver -lmongoclient -lpthread -lboost_thread -lboost_filesystem -lboost_system	
    endif
clean:
	rm *.o