# Makefile NOTE this makefile is not done nor has it been modidiet for our project yet!!!
CC = g++
CFLAGS = -g -Wall -std=c++11 
OFILES = testShape.o shape.o eatFruit.o
all: eatFruit
fruits.o : fruits.cc fruits.h 
$(CC) $(CFLAGS) -c $< -o $@
persons.o : persons.cc persons.h fruits.h 
$(CC) $(CFLAGS) -c $< -o $@
eatFruits.o : eatFruit.cc fruits.h persons.h 
$(CC) $(CFLAGS) -c $< -o $@
eatFruit : $(OFILES)
$(CC) $(OFILES) -o $@
clean:
@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
@/bin/rm -f *.o *.x *.cc~ *.h~