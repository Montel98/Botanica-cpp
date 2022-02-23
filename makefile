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
	g++ -c src/test.cpp -o bin/Test.o

	g++ bin/Test.o bin/Leaf.o bin/FourierSeries.o bin/Geometry.o bin/ParametricGeometry.o bin/BufferAttributes.o bin/BezierCubic.o src/glad.o -lGL -lGLEW -lGLU -lglfw -ldl -o bin/OpenGLTest
