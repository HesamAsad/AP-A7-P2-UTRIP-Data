#ifndef RESERVATION_H
#define RESERVATION_H
#include <string>
#include <iostream>
#include "Reservation_date.h"
#include "User.h"
class Hotel;
class Room;

class Reservation
{
public:
	Reservation(User* _user, int _uid, Hotel* _hotel, Reservation_date* _date, std::vector<Room*> _rooms);
	~Reservation() { delete rdate; }
	int get_price();
	void print_success();
	void print();
	void clean();
	inline int get_uid() { return uid; }
	inline Reservation_date get_date() { return *rdate; }
	inline int get_rooms_count() { return rooms.size(); }
	double get_pure_price();
private:
	Hotel* hotel;
	Reservation_date* rdate;
	int uid;
	User* user;
	std::vector<Room*> rooms;
};

#endif