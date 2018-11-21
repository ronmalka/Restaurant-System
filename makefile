# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: rest

# Tool invocations
# Executable "rest" depends on the files Table.o, Action.o, Customer.o, Restaurant.o and Dish.o  
rest: bin/Table.o bin/Action.o bin/Customer.o bin/Restaurant.o bin/Dish.o bin/Main.o
	@echo 'Building target: rest'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/rest bin/Table.o bin/Action.o bin/Customer.o bin/Restaurant.o bin/Dish.o bin/Main.o 
	@echo 'Finished building target: rest'
	@echo ' '

# Depends on the source and header files
bin/Table.o: src/Table.cpp include/Table.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Table.o src/Table.cpp

# Depends on the source and header files 
bin/Action.o: src/Action.cpp include/Action.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp

# Depends on the source and header files 
bin/Customer.o: src/Customer.cpp include/Customer.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp

# Depends on the source and header files 
bin/Restaurant.o: src/Restaurant.cpp include/Restaurant.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

# Depends on the source and header files 
bin/Dish.o: src/Dish.cpp include/Dish.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Dish.o src/Dish.cpp

# Depends on the source file 
bin/Main.o: src/Main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp

#Clean the build directory
clean: 
	rm -f bin/*
