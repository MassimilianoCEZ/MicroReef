# Definitions de macros

CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = shape.cc message.cc microReef.cc testShape.cc
OFILES = $(CXXFILES:.cc=.o)

# Definition de la premiere regle
# this has the name "testShape" of the executable file

testShape: $(OFILES)
	$(CXX) $(OFILES) -o testShape

# Definitions de cibles particulieres

depend:
	@echo " *** UPATE OF DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

# -- deletes object modules and executables (use command: "make clean")

clean:
	@echo " *** ERASE OBJET MODULE(S) AND EXECUTABLE(S) ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ testShape

#
# -- Automatically generated dependancies (use command: "make depend")
# -- To run program use command: "./testshape"
#
# DO NOT DELETE THIS LINE
shape.o: shape.cc shape.h
message.o: message.cc message.h
microReef.o: microReef.cc
testShape.o: testShape.cc shape.h
