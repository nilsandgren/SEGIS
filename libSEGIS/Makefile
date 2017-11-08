CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix objs/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := 
CC_FLAGS := -g -Wall -Werror -O2 -std=c++11 -Iinclude -I/usr/include/freetype2

libSEGIS.a: $(OBJ_FILES)
	ar rcs $@ $^
	ranlib $@
	chmod a+x $@

objs/%.o: src/%.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

clean:
	$(RM) objs/*.o
	$(RM) libSEGIS.a

