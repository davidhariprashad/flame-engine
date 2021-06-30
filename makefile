CXX = g++
CPPFLAGS = -Wextra -O3 -std=c++17 -Wno-psabi
LDFLAGS = -lpthread

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

executable : $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

obj : $(wildcard *.cpp)
	$(CXX) -c $(CPPFLAGS) -o foo $(objects)

clean :
	rm *.o executable
