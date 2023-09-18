render: main.o model.o image.o
	g++  bin/main.o bin/model.o bin/image.o -o bin/render -g
main.o: main.cpp 
	g++ -c main.cpp -o bin/main.o -g
model.o: Model.cpp
	g++ -c Model.cpp -o bin/model.o -g
image.o: Image.cpp
	g++ -c Image.cpp -o bin/image.o -g