#include "Rooms.h"
#include "Utrip.h"
#include <iostream>
using namespace std;

int Std_room::uid_num = 1;
int Deluxe_room::uid_num = 1;
int Lux_room::uid_num = 1;
int Prem_room::uid_num = 1;

void Room::reset_uid_num()
{
	Std_room::uid_num = 1;
	Deluxe_room::uid_num = 1;
	Lux_room::uid_num = 1;
	Prem_room::uid_num = 1;
}

Rooms_info Room::read_rooms_info(std::vector<std::string>& hotel_data)
{
	Rooms_info rooms_info;

	rooms_info.std_count = stoi(hotel_data[STD_ROOMS_COUNT]);
	rooms_info.deluxe_count = stoi(hotel_data[DELUXE_ROOMS_COUNT]);
	rooms_info.lux_count = stoi(hotel_data[LUX_ROOMS_COUNT]);
	rooms_info.prem_count = stoi(hotel_data[PREM_ROOMS_COUNT]);
	rooms_info.std_price = stoi(hotel_data[STD_ROOMS_PRICE]);
	rooms_info.deluxe_price = stoi(hotel_data[DELUXE_ROOMS_PRICE]);
	rooms_info.lux_price = stoi(hotel_data[LUX_ROOMS_PRICE]);
	rooms_info.prem_price = stoi(hotel_data[PREM_ROOMS_PRICE]);
	return rooms_info;
}

int Room::calc_price(vector<Room*>& rooms)
{
	int price = 0;
	for (Room*& room : rooms)
		price += room->price;
	return price;
}

bool Room::is_available_in(const Reservation_date& date)
{
	for (Reservation*& reserve : reservations)
		if (Reservation_date::dates_overlap(reserve->get_date(), date))
			return false;
	return true;
}

void Room::delete_reservation(Reservation* reserve)
{
	vector<Reservation*> updated;
	for (Reservation*& reservation : reservations)
		if (reservation == reserve)
			continue;
		else
			updated.push_back(reservation);
	if(reservations.size() == updated.size())
		throw Not_found();
	reservations.clear();
	reservations = updated;
}