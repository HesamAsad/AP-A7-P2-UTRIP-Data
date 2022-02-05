#include "Hotel.h"
#include "Exceptions.h"
#include "Reservation_date.h"
#include <iostream>
#include <iomanip>
using namespace std;

Hotel::Hotel(std::string _uid, std::string _name, int _stars, std::string _review,
	std::string _amenities, std::string _city, double _latitude, double _longtitude,
	std::string _image_url, Rooms_info _rooms_info, std::vector<Room*> _rooms)
{
	uid = _uid; name = _name; stars = _stars;
	review = _review; amenities = _amenities;
	city = _city; latitude = _latitude;
	longtitude = _longtitude;  image_url = _image_url;
	rooms_info = _rooms_info;  rooms = _rooms;
	total_rooms = rooms_info.std_count + rooms_info.deluxe_count
		+ rooms_info.lux_count + rooms_info.prem_count;
	non_zero_rooms = count_non_zero_rooms(rooms_info);
	total_price = rooms_info.std_price + rooms_info.deluxe_price
		+ rooms_info.lux_price + rooms_info.prem_price;
	if (non_zero_rooms == 0)
		average_price = 0;
	else
		average_price = total_price / non_zero_rooms;
}

void Hotel::print_complete()
{
	cout << uid << endl;
	cout << name << endl;
	cout << "star: " << stars << endl;
	cout << "overview: " << review << endl;
	cout << "amenities: " << amenities << endl;
	cout << "city: " << city << endl;
	cout << "latitude: " << fixed << setprecision(2) << latitude << endl;
	cout << "longitude: " << fixed << setprecision(2) << longtitude << endl;
	cout << "#rooms: " << rooms_info.std_count << ' '
		<< rooms_info.deluxe_count << ' '
		<< rooms_info.lux_count << ' '
		<< rooms_info.prem_count << endl;
	cout << "price: " << rooms_info.std_price << ' '
		<< rooms_info.deluxe_price << ' '
		<< rooms_info.lux_price << ' '
		<< rooms_info.prem_price << endl;
}

void Hotel::print()
{
	cout << uid << ' ' << name << ' ' << stars << ' ' << city << ' ';
	cout << total_rooms << ' ';
	cout << fixed << setprecision(2) << average_price << ' ' << endl;
}

int Hotel::count_non_zero_rooms(Rooms_info rooms_info)
{
	int non_zero_rooms = 0;
	if (rooms_info.std_price)
		non_zero_rooms++;
	if (rooms_info.deluxe_price)
		non_zero_rooms++;
	if (rooms_info.lux_price)
		non_zero_rooms++;
	if (rooms_info.prem_price)
		non_zero_rooms++;
	return non_zero_rooms;
}

void Hotel::clear()
{
	for (Room*& room : rooms)
		delete room;
}

vector<Room*> Hotel::find_available_rooms(int quantity, string type, const Reservation_date& rdate)
{
	vector<Room*> available;
	for (Room*& room : rooms)
	{
		if ((room->what_are_you() == type) && room->is_available_in(rdate))
			available.push_back(room);
		if (available.size() == quantity)
			return available;
	}
	throw Not_enough_room();
}

std::vector<Room*> Hotel::get_room_by_type(std::string type)
{
	vector<Room*> result;
	for (Room*& room : rooms)
		if (room->what_are_you() == type)
			result.push_back(room);
	return result;
}

void Hotel::add_comment(string user_name, string comment)
{
	comments.push_back(make_pair(user_name, comment));
}

Hotel::~Hotel()
{
	for (Rating*& rating : ratings)
		delete rating;
}

void Hotel::show_comments()
{
	for (auto pair = comments.rbegin(); pair != comments.rend(); pair++)
		cout << pair->first << ": " << pair->second << endl;
}

void Hotel::show_rating()
{
	if (ratings.empty())
	{
		cout << NO_RATING << endl;
		return;
	}
	Rating average = find_average_rating();
	
	cout << "location: " << fixed << setprecision(2) << average.location << '\n';
	cout << "cleanliness: " << fixed << setprecision(2) << average.cleanliness << '\n';
	cout << "staff: " << fixed << setprecision(2) << average.staff << '\n';
	cout << "facilities: " << fixed << setprecision(2) << average.facilities << '\n';
	cout << "value_for_money: " << fixed << setprecision(2) << average.value_for_money << '\n';
	cout << "overall_rating: " << fixed << setprecision(2) << average.overall << '\n';
}

void Hotel::add_rating(Rating* _rating)
{
	if (_rating->get_user() == NULL)
	{
		ratings.push_back(_rating);
		return;
	}
	for (Rating*& rating : ratings)
		if (rating->get_user() == _rating->get_user())
		{
			delete rating;
			rating = _rating;
			return;
		}
	ratings.push_back(_rating);
}

double Hotel::get_overall_rating()
{
	return find_average_rating().overall;
}

Rating Hotel::find_average_rating()
{
	for (Rating*& rating : ratings)
		if (rating->get_user() == NULL)
			return *rating;
}

double Hotel::get_personal_rating(User* user)
{
	for (Rating*& rating : ratings)
		if (rating->get_user() == user)
			return rating->overall;

	Rating hotel_avg = find_average_rating();
	Rating* user_manual_weights = user->get_manual_weights();
	return Rating::get_weighted_avg(user_manual_weights, &hotel_avg);
}

Rating* Hotel::get_user_rating(User* user)
{
	for (Rating* rating : ratings)
		if (rating->get_user() == user)
			return rating;
	return NULL;
}

double Hotel::get_estimated_rating(User* user, std::vector<double> estimated_weights)
{
	for (Rating*& rating : ratings)
		if (rating->get_user() == user)
			return rating->overall;
	Rating hotel_avg = find_average_rating();
	return Rating::get_weighted_avg(estimated_weights, &hotel_avg);
}