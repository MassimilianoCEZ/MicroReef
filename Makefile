#
# Ferrulli Massimiliano
# Waldorff Carl Johan Traeholt
# version 64
#

CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = shape.cc message.cc lifeform.cc simulation.cc projet.cc
OFILES = $(CXXFILES:.cc=.o)

projet: $(OFILES)
	$(CXX) $(OFILES) -o projet


depend:
	@echo " *** UPATE OF DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile



clean:
	@echo " *** ERASE OBJET MODULE(S) AND EXECUTABLE(S) ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ testShape


# DO NOT DELETE THIS LINE
shape.o: shape.cc shape.h constantes.h
message.o: message.cc message.h
lifeform.o: lifeform.cc Lifeform.h shape.h constantes.h message.h
simulation.o: simulation.cc simulation.h lifeform.h shape.h constantes.h \
 message.h
projet.o: projet.cc simulation.h lifeform.h shape.h constantes.h \
 message.h
