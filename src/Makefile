CXX = g++ -std=c++11
MAIN = utrip.out

all: $(MAIN)
	@echo Compiled $(MAIN)

$(MAIN): Estimated_weights.o Filter.o Hotel.o Interface.o main.o Rating.o Reservation.o Reservation_date.o Rooms.o Sort.o User.o Utrip.o 
	$(CXX) Estimated_weights.o Filter.o Hotel.o Interface.o main.o Rating.o Reservation.o Reservation_date.o Rooms.o Sort.o User.o Utrip.o -o $(MAIN)

Estimated_weights.o: Estimated_weights.cpp Estimated_weights.h Exceptions.h Utrip.h User.h Hotel.h Rating.h
	$(CXX) -c Estimated_weights.cpp

Filter.o: Filter.h Filter.cpp Hotel.h Exceptions.h
	$(CXX) -c Filter.cpp

Hotel.o: Hotel.h Hotel.cpp Exceptions.h Reservation_date.h Rooms.h Rating.h
	$(CXX) -c Hotel.cpp

Interface.o: Interface.cpp Interface.h Exceptions.h Utrip.h
	$(CXX) -c Interface.cpp

main.o: main.cpp Utrip.h Interface.h 
	$(CXX) -c main.cpp

Rating.o: Rating.h Rating.cpp User.h Exceptions.h 
	$(CXX) -c Rating.cpp

Reservation_date.o: Reservation_date.h Reservation_date.cpp Exceptions.h 
	$(CXX) -c Reservation_date.cpp

Reservation.o: Reservation.h Reservation.cpp User.h Hotel.h Reservation_date.h
	$(CXX) -c Reservation.cpp

Rooms.o: Rooms.h Rooms.cpp Utrip.h Reservation.h 
	$(CXX) -c Rooms.cpp

Sort.o: Sort.h Sort.cpp Hotel.h 
	$(CXX) -c Sort.cpp

User.o: User.h User.cpp Reservation.h Hotel.h 
	$(CXX) -c User.cpp

Utrip.o: Utrip.h Utrip.cpp Exceptions.h Reservation.h Interface.h Hotel.h User.h Filter.h Sort.h Estimated_weights.h 
	$(CXX) -c Utrip.cpp

clean:
	$(RM) *.o .*~ $(MAIN)
