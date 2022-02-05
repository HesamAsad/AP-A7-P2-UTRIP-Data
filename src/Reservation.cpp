#include "Reservation.h"
#include "Hotel.h"
#include <iomanip>
using namespace std;

Reservation::Reservation(User* _user, int _uid, Hotel* _hotel, Reservation_date* _date, std::vector<Room*> _rooms)
{
	hotel = _hotel;
	rdate = _date;
	rooms = _rooms;
	uid = _uid;
	user = _user;
	for (Room*& room : rooms)
		room->reserve(this);
}

void Reservation::print_success()
{
	for (unsigned int i = 0; i < rooms.size(); i++)
		cout << rooms[i]->get_uid() << ' ';
	cout << endl;
}

int Reservation::get_price()
{
	int price = 0;
	for (Room*& room : rooms)
		price += (room->get_price() * rdate->get_days());
	return price;
}

void Reservation::print()
{
	cout << "id: " << uid << ' ';
	cout << "hotel: " << hotel->get_uid() << ' ';
	cout << "room: " << rooms[0]->what_are_you() << ' ';
	cout << "quantity: " << rooms.size() << ' ';
	cout << "cost: " << get_price() << ' ';
	cout << "check_in " << rdate->get_checkin() << ' ';
	cout << "check_out " << rdate->get_checkout() << endl;
}

void Reservation::clean()
{
	for (Room*& room : rooms)
		room->delete_reservation(this);
}

double Reservation::get_pure_price()
{
	return rooms[0]->get_price() * rooms.size();
}