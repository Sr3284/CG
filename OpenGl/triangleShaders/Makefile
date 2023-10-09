CXX = g++
VERSION = -std=c++11
LIBS = -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
SRC_PATH = src/cpp

bin/hello: $(SRC_PATH)/hello.cpp
	$(CXX) $(SRC_PATH)/hello.cpp -o bin/hello $(VERSION) -Wall $(LIBS)

.PHONY: clean

clean:
	rm -f bin/*.o bin/hello