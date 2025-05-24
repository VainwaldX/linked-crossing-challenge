# Makefile
OUT = projet
CXX = g++
CXXFLAGS = -Wall -std=c++17 -m64
LINKING = `pkg-config --cflags gtkmm-4.0`
LDLIBS = `pkg-config --libs gtkmm-4.0`
CXXFILES = projet.cc gui.cc graphic.cc jeu.cc tools.cc message.cc mobile.cc chaine.cc
OFILES = $(CXXFILES:.cc=.o)

all: $(OUT)

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ $(OUT)

tools.o : tools.cc tools.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

message.o : message.cc message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

chaine.o   :  chaine.cc chaine.h tools.h message.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 
	
mobile.o : mobile.cc mobile.h tools.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

jeu.o :  jeu.cc jeu.h mobile.h chaine.h tools.h message.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

graphic.o: graphic.cc graphic_gui.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

gui.o: gui.cc graphic_gui.h graphic.h gui.h jeu.h tools.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

projet.o: projet.cc gui.h jeu.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ 

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)
