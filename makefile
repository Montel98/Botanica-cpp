default:
	g++ -c src/glad.c
	g++ -c src/FourierSeries.cpp -o bin/FourierSeries.o
	g++ -c src/BezierLinear.cpp -o bin/BezierLinear.o
	g++ -c src/BezierCubic.cpp -o bin/BezierCubic.o
	g++ -c src/Leaf.cpp -o bin/Leaf.o
	g++ -c src/Geometry.cpp -o bin/Geometry.o
	g++ -c src/ParametricGeometry.cpp -o bin/ParametricGeometry.o
	g++ -c src/Entity.cpp -o bin/Entity.o
	g++ -c src/BufferAttributes.cpp -o bin/BufferAttributes.o
	g++ -c src/Texture.cpp -o bin/Texture.o
	g++ -c src/Material.cpp -o bin/Material.o
	g++ -c src/Mesh.cpp -o bin/Mesh.o
	g++ -c src/Stem.cpp -o bin/Stem.o
	g++ -c src/StemBuilder.cpp -o bin/StemBuilder.o
	g++ -c src/EntityManager.cpp -o bin/EntityManager.o
	g++ -c src/Object3D.cpp -o bin/Object3D.o
	g++ -c src/Axis.cpp -o bin/Axis.o
	g++ -c src/LSystem.cpp -o bin/LSystem.o
	g++ -c src/LSystemRules.cpp -o bin/LSystemRules.o
	g++ -c src/test.cpp -o bin/Test.o

	g++ bin/Test.o bin/Mesh.o bin/Entity.o bin/Leaf.o bin/FourierSeries.o bin/Geometry.o bin/ParametricGeometry.o bin/BufferAttributes.o bin/Texture.o bin/Material.o bin/BezierCubic.o bin/BezierLinear.o bin/Stem.o bin/StemBuilder.o bin/EntityManager.o bin/Object3D.o bin/Axis.o bin/LSystem.o bin/LSystemRules.o src/glad.o -lGL -lGLEW -lGLU -lglfw -ldl -o bin/OpenGLTest
