sim: Simulator.o Base_Station.o User.o Location_Area.o
	g++ Simulator.o Base_Station.o User.o Location_Area.o -o sim

Simulator.o: Simulator.cpp
	g++ -c Simulator.cpp

Base_Station.o: Base_Station.cpp
	g++ -c Base_Station.cpp

User.o: User.cpp
	g++ -c User.cpp

Location_Area.o: Location_Area.cpp
	g++ -c Location_Area.cpp

clean:
	-rm *.o sim