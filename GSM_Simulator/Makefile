sim: Simulator.o Base_Station.o
	g++ Simulator.o Base_Station.o -o sim

Simulator.o: Simulator.cpp
	g++ -c Simulator.cpp

Base_Station.o: Base_Station.cpp
	g++ -c Base_Station.cpp

clean:
	-rm *.o sim
