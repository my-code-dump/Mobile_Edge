sim: Simulator.o Base_Station.o User.o
	g++ Simulator.o Base_Station.o User.o -o sim

Simulator.o: Simulator.cpp
	g++ -c Simulator.cpp

Base_Station.o: Base_Station.cpp
	g++ -c Base_Station.cpp

User.o: User.cpp
	g++ -c User.cpp

clean:
	-rm *.o sim