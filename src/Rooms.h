#ifndef ROOMS_H
#define ROOMS_H
#include <string>
#include <vector>
#include "Reservation.h"

const std::string std_uid_string = "s";
const std::string deluxe_uid_string = "d";
const std::string lux_uid_string = "l";
const std::string prem_uid_string = "p";

#define STDROOMTYPE "standard"
#define DELUXEROOMTYPE "deluxe"
#define LUXROOMTYPE "luxury"
#define PREMROOMTYPE "premium"

typedef struct
{
	int std_count, std_price;
	int deluxe_count, deluxe_price;
	int lux_count, lux_price;
	int prem_count, prem_price;
} Rooms_info;


class Room
{
public:
	virtual ~Room() = default;
	static void reset_uid_num();
	static Rooms_info read_rooms_info(std::vector<std::string>& hotel_data);
	static int calc_price(std::vector<Room*>& rooms);
	inline int get_price() { return price; }
	inline std::string get_uid() { return uid; }
	bool is_available_in(const Reservation_date& date);
	void reserve(Reservation* reservation) { reservations.push_back(reservation); }
	void delete_reservation(Reservation* reserve);
	virtual std::string what_are_you() = 0;
protected:
	int price;
	std::string uid;
	std::vector<Reservation*> reservations;
};

class Std_room : public Room
{
public:
	Std_room(int _price)
	{
		price = _price;
		uid = std_uid_string + std::to_string(uid_num++);
	}
	inline std::string what_are_you() { return STDROOMTYPE; }
	static int uid_num;
};

class Deluxe_room : public Room
{
public:
	Deluxe_room(int _price)
	{
		price = _price;
		uid = deluxe_uid_string + std::to_string(uid_num++);
	}
	static int uid_num;
	inline std::string what_are_you() { return DELUXEROOMTYPE; }
};

class Lux_room : public Room
{
public:
	Lux_room(int _price)
	{
		price = _price;
		uid = lux_uid_string + std::to_string(uid_num++);
	}
	static int uid_num;
	inline std::string what_are_you() { return LUXROOMTYPE; }
};

class Prem_room : public Room
{
public:
	Prem_room(int _price)
	{
		price = _price;
		uid = prem_uid_string + std::to_string(uid_num++);
	}
	static int uid_num;
	inline std::string what_are_you() { return PREMROOMTYPE; }
};

#endif