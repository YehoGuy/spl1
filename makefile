# All Targets

all: clean compile link check_leaks




clean:
    rm -f bin/*

compile:
	g++ -g -Wall -Weffc++ -c -o bin/Customer.o src/Customer.cpp
	g++ -g -Wall -Weffc++ -c -o bin/Volunteer.o src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -c -o bin/Order.o src/Order.cpp
	g++ -g -Wall -Weffc++ -c -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -c -o bin/WareHouse.o src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -c -o bin/main.o src/main.cpp
    
link:
	g++ -o bin/program bin/Customer.o bin/Volunteer.o bin/Order.o bin/Action.o bin/WareHouse.o bin/main.o

check_leaks:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/program


