CXX = g++
VERSION = -std=c++11
LIBS = -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor

mandelbrot: mandelbrot.cpp
	$(CXX) $(VERSION) mandelbrot.cpp -o mandelbrot $(LIBS)

.PHONY: clean

clean:
	rm -rf mandelbrot