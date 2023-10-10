CXX = g++
LIBS = -lGLEW -lGL -lGLU -lglut

tetrahedron/tetrahedron: tetrahedron/tetrahedron.cpp
	$(CXX) tetrahedron/tetrahedron.cpp -o tetrahedron/tetra -Wall $(LIBS)

torus/torus: torus/torus.cpp
	$(CXX) torus/torus.cpp -o torus/torus -Wall $(LIBS)

spinCube/spinCube: spinCube/spinCube.cpp
	$(CXX) spinCube/spinCube.cpp -o spinCube/spin -Wall $(LIBS)

.PHONY: clean

clean:
	rm -rf tetrahedron/tetra
	rm -rf torus/torus
	rm -rf spinCube/spin
