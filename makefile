default:
	g++ -std=c++2a -c src/glad.c
	g++ -std=c++2a -c src/FourierSeries.cpp -o bin/FourierSeries.o
	g++ -std=c++2a -c src/BezierLinear.cpp -o bin/BezierLinear.o
	g++ -std=c++2a -c src/BezierCubic.cpp -o bin/BezierCubic.o
	g++ -std=c++2a -c src/Leaf.cpp -o bin/Leaf.o
	g++ -std=c++2a -c src/Geometry.cpp -o bin/Geometry.o
	g++ -std=c++2a -c src/ParametricGeometry.cpp -o bin/ParametricGeometry.o
	g++ -std=c++2a -c src/Entity.cpp -o bin/Entity.o
	g++ -std=c++2a -c src/BufferAttributes.cpp -o bin/BufferAttributes.o
	g++ -std=c++2a -c src/Texture.cpp -o bin/Texture.o
	g++ -std=c++2a -c src/Material.cpp -o bin/Material.o
	g++ -std=c++2a -c src/Mesh.cpp -o bin/Mesh.o
	g++ -std=c++2a -c src/Stem.cpp -o bin/Stem.o
	g++ -std=c++2a -c src/StemBuilder.cpp -o bin/StemBuilder.o
	g++ -std=c++2a -c src/EntityManager.cpp -o bin/EntityManager.o
	g++ -std=c++2a -c src/Object3D.cpp -o bin/Object3D.o
	g++ -std=c++2a -c src/Axis.cpp -o bin/Axis.o
	g++ -std=c++2a -c src/LSystem.cpp -o bin/LSystem.o
	g++ -std=c++2a -c src/LSystemRules.cpp -o bin/LSystemRules.o
	g++ -std=c++2a -c src/LSystemOps.cpp -o bin/LSystemOps.o
	g++ -std=c++2a -c src/test.cpp -o bin/Test.o

	g++ -std=c++2a bin/Test.o bin/Mesh.o bin/Entity.o bin/Leaf.o bin/FourierSeries.o bin/Geometry.o bin/ParametricGeometry.o bin/BufferAttributes.o bin/Texture.o bin/Material.o bin/BezierCubic.o bin/BezierLinear.o bin/Stem.o bin/StemBuilder.o bin/EntityManager.o bin/Object3D.o bin/Axis.o bin/LSystem.o bin/LSystemRules.o bin/LSystemOps.o src/glad.o -lGL -lGLEW -lGLU -lglfw -ldl -o bin/OpenGLTest
