CXX = g++
LIBS = -lGLEW -lGL -lGLU -lglut

all: tetrahedron/tetrahedron torus/torus spinCube/spinCube comet/comet moon/moon bounce/bounce

tetrahedron/tetrahedron: tetrahedron/tetrahedron.cpp
	$(CXX) tetrahedron/tetrahedron.cpp -o tetrahedron/tetra -Wall $(LIBS)

torus/torus: torus/torus.cpp
	$(CXX) torus/torus.cpp -o torus/torus -Wall $(LIBS)

spinCube/spinCube: spinCube/spinCube.cpp
	$(CXX) spinCube/spinCube.cpp -o spinCube/spin -Wall $(LIBS)

comet/comet: comet/comet.cpp
	$(CXX) comet/comet.cpp -o comet/comet -Wall $(LIBS)

moon/moon: moon/moon.cpp
	$(CXX) moon/moon.cpp -o moon/moon -Wall $(LIBS)

bounce/bounce: bounce/bounce.cpp
	$(CXX) bounce/bounce.cpp -o bounce/bounce -Wall $(LIBS)

.PHONY: clean

clean:
	rm -rf tetrahedron/tetra
	rm -rf torus/torus
	rm -rf spinCube/spin
	rm -rf comet/comet
	rm -rf moon/moon
	rm -rf bounce/bounce