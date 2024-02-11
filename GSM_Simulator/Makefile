sim: Simulator.o Base_Station.o User.o Location_Area.o
	g++ -std=c++11 Simulator.o Base_Station.o User.o Location_Area.o -o sim

Simulator.o: Simulator.cpp
	g++ -std=c++11 -c Simulator.cpp

Base_Station.o: Base_Station.cpp
	g++ -std=c++11 -c Base_Station.cpp

User.o: User.cpp
	g++ -std=c++11 -c User.cpp

Location_Area.o: Location_Area.cpp
	g++ -std=c++11 -c Location_Area.cpp

clean:
	-rm *.o sim