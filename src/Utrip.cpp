#include "Utrip.h"
#include "Exceptions.h"
#include "Reservation.h"
#include "Interface.h"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <iostream>
#include <numeric>
using namespace std;

Utrip::Utrip()
{
	user_logged_in = NULL;
	personal_sort = NULL;
	signed_in = false;
	default_price_filter = false;
	user_sort = false;
	estimated_weights_calculated = false;
	manual_weights = false;
	personal_sort_mod = false;
}

vector<string> Utrip::split(const string &str, char delim)
{
	size_t start_pos = 0, delim_pos;
	string sub;

	vector<string> splitted;

	while ((delim_pos = str.find(delim, start_pos)) != string::npos)
	{
		sub = str.substr(start_pos, delim_pos - start_pos);
		splitted.push_back(sub);
		start_pos = delim_pos + 1;
	}

	if (str[start_pos] != '\0')
	{
		sub = str.substr(start_pos);
		splitted.push_back(sub);
	}

	return splitted;
}

void Utrip::load_hotels(string assets_path)
{
	ifstream infile(assets_path);
	string buffer;
	getline(infile, buffer);

	while (getline(infile, buffer))
	{
		vector<string> data = split(buffer, CSV_DELIM);
		Rooms_info rooms_info = Room::read_rooms_info(data);
		vector<Room*> loaded_rooms = read_rooms(rooms_info);

		hotels.push_back(Hotel(data[ID], data[NAME],
			stoi(data[STARS]), data[OVERVIEW],
			data[AMENITIES], data[CITY],
			stod(data[LATITUDE]), stod(data[LONGTITUDE]),
			data[IMAGE_URL], rooms_info, loaded_rooms));
	}
	initialize_filtered_hotels();
}

std::vector<Room*> Utrip::read_rooms(Rooms_info& info)
{
	int i;
	vector<Room*> rooms;

	for (i = 0; i < info.std_count; i++)
		rooms.push_back(new Std_room(info.std_price));

	for (i = 0; i < info.deluxe_count; i++)
		rooms.push_back(new Deluxe_room(info.deluxe_price));

	for (i = 0; i < info.lux_count; i++)
		rooms.push_back(new Lux_room(info.lux_price));

	for (i = 0; i < info.prem_count; i++)
		rooms.push_back(new Prem_room(info.prem_price));

	Room::reset_uid_num();
	return rooms;
}

void Utrip::signup_user(map<string, string> arguments)
{
	if (signed_in)
		throw Bad_request();
	string username, email, pass;
	for (auto argument = arguments.begin(); argument != arguments.end(); argument++)
	{
		string key = argument->first, value = argument->second;
		if (key == PASS_ARG)
			pass = value;
		else if (key == USERNAME_ARG)
			username = value;
		else if (key == EMAIL_ARG)
			email = value;
		else
			throw Bad_request();
	}
	if (user_exists(email, username))
		throw Bad_request();
	users.push_back(new User(username, email, pass));
	user_logged_in = users.back();
	signed_in = true;
	COMMAND_EXECUTED;
}

void Utrip::login_user(map<string, string> arguments)
{
	if (signed_in)
		throw Bad_request();
	string email, pass;
	for (auto argument = arguments.begin(); argument != arguments.end(); argument++)
	{
		string key = argument->first, value = argument->second;
		if (key == PASS_ARG)
			pass = value;
		else if (key == EMAIL_ARG)
			email = value;
		else
			throw Bad_request();
	}
	User* user = find_user(email);
	if (!user->is_password_true(pass))
		throw Bad_request();
	user_logged_in = user;
	signed_in = true;
	if (user_logged_in->has_enough_reservations())
		default_price_filter = true;
	COMMAND_EXECUTED;
}

User* Utrip::find_user(string email)
{
	for (User* user : users)
		if (user->get_email() == email)
			return user;
	throw Bad_request();
}

void Utrip::add_to_wallet(double amount)
{
	check_signed_in();
	if (amount <= 0)
		throw Bad_request();

	user_logged_in->add_to_wallet(amount);
	COMMAND_EXECUTED;
}

bool Utrip::user_exists(string email, string username)
{
	for (User* user : users)
		if (user->get_email() == email || user->get_username() == username)
			return true;
	return false;
}

void Utrip::logout_user()
{
	check_signed_in();
	user_logged_in = NULL;
	signed_in = false;
	manual_weights = false;
	if (personal_sort)
	{
		delete personal_sort;
		personal_sort = NULL;
	}
	estimated_weights_for_user.clear();
	estimated_weights_calculated = false;
	initialize_filtered_hotels();
	COMMAND_EXECUTED;
}

void Utrip::show_hotels()
{
	check_signed_in();
	if (filtered_hotels.empty())
	{
		EMPTY_ERROR;
		return;
	}
	vector<Hotel*> to_be_printed = get_printable_hotels();
	if (to_be_printed.empty())
	{
		EMPTY_ERROR;
		return;
	}
	if (default_price_filter)
		cout << "* Results have been filtered by the default price filter." << endl;
	sort_hotels(to_be_printed);
	for (Hotel* hotel : to_be_printed)
		hotel->print();

}

void Utrip::show_hotel(string uid)
{
	check_signed_in();
	for (Hotel& hotel : hotels)
		if (hotel.get_uid() == uid)
		{
			hotel.print_complete();
			return;
		}
	throw Not_found();
}

void Utrip::initialize_filtered_hotels()
{
	for (auto filter : all_filters)
		delete filter.first;
	all_filters.clear();
	filtered_hotels.clear();
	for (Hotel& hotel : hotels)
		filtered_hotels.push_back(&hotel);
}

void Utrip::city_filter(std::string city)
{
	check_signed_in();
	if (filter_applied(CITY_TYPE))
		update_filtered_hotels(new City_filter(city));
	else
	{
		City_filter* filter = new City_filter(city);
		all_filters[filter] = filter->apply(hotels);
		intersect_filters();
	}
}

void Utrip::stars_filter(int min, int max)
{
	check_signed_in();
	if (min > max || min < 1 || max > 5)
		throw Bad_request();
	if (filter_applied(STAR_TYPE))
		update_filtered_hotels(new Stars_filter(min, max));
	else
	{
		Stars_filter* filter = new Stars_filter(min, max);
		all_filters[filter] = filter->apply(hotels);
		intersect_filters();
	}
}

void Utrip::price_filter(int min, int max)
{
	check_signed_in();
	if (min > max || min < 0 || max < 0)
		throw Bad_request();
	default_price_filter = false;
	if (filter_applied(PRICE_TYPE))
		update_filtered_hotels(new Price_filter(min, max));
	else
	{
		Price_filter* filter = new Price_filter(min, max);
		all_filters[filter] = filter->apply(hotels);
		intersect_filters();
	}
}

void Utrip::compound_filter(string type, int quantity, int checkin, int checkout)
{
	check_signed_in();
	if (type == "" || quantity <= 0 || checkin <= 0 || checkin > 30 || checkout <= 0 || checkout > 30)
		throw Bad_request();
	if (filter_applied(COMPOUND_TYPE))
		update_filtered_hotels(new Compound_filter(type, quantity, checkin, checkout));
	else
	{
		Compound_filter* filter = new Compound_filter(type, quantity, checkin, checkout);
		all_filters[filter] = filter->apply(hotels);
		intersect_filters();
	}
}

bool Utrip::filter_applied(string type)
{
	for (auto filter : all_filters)
		if ((filter.first)->get_type() == type)
			return true;
	return false;
}

void Utrip::delete_filter()
{
	check_signed_in();
	initialize_filtered_hotels();
	if (user_logged_in->has_enough_reservations())
		default_price_filter = true;
	COMMAND_EXECUTED;
}

void Utrip::update_filtered_hotels(Filter* filter)
{
	Filter* to_be_deleted;
	for (auto _filter : all_filters)
		if ((_filter.first)->get_type() == filter->get_type())
			to_be_deleted = _filter.first;
	delete to_be_deleted;
	all_filters.erase(to_be_deleted);
	vector<Hotel*> new_filtered = filter->apply(hotels);
	all_filters[filter] = new_filtered;
	intersect_filters();
}

void Utrip::intersect_filters()
{
	vector<vector<Hotel*>> all;
	for (auto filtered : all_filters)
		all.push_back(filtered.second);
	filtered_hotels = give_intersection(all);
	COMMAND_EXECUTED;
}

vector<Hotel*> Utrip::give_intersection(vector<vector<Hotel*>> all, int i)
{
	if (all.size() == 1)
		return all[0];
	if (all.size() == 2)
		return give_intersection(all[0], all[1]);
	if (all.size() - i != 2)
	{
		vector<Hotel*> intersection = give_intersection(all, i + 1);
		return give_intersection(intersection, all[i]);
	}
	else
		return give_intersection(all[i], all[i + 1]);
}

vector<Hotel*> Utrip::give_intersection(vector<Hotel*> v1, vector<Hotel*> v2)
{
	vector<Hotel*> intersection(v1.size() + v2.size());
	vector<Hotel*>::iterator end;
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	end = set_intersection(v1.begin(), v1.end(),
		v2.begin(), v2.end(), intersection.begin());
	intersection.resize(end - intersection.begin());
	return intersection;
}

Utrip::~Utrip()
{
	for (Hotel& hotel : hotels)
		hotel.clear();
	for (User*& user : users)
		delete user;
	for (auto filter : all_filters)
		delete filter.first;
	if (personal_sort)
		delete personal_sort;
}

void Utrip::reserve(map<string, string> arguments)
{
	check_signed_in();
	Reservation_date* rdate;
	try
	{
		Reserve_arguments args = read_reservation_args(arguments);
		check_room_type(args.type);
		Hotel* hotel = find_hotel(args.hotel_uid);
		int uid = user_logged_in->get_reservation_uid();
		rdate = new Reservation_date(args.checkin, args.checkout);
		vector<Room*> rooms = hotel->find_available_rooms(args.quantity, args.type, *rdate);
		if (user_logged_in->has_money(rdate->get_days() * Room::calc_price(rooms)))
			user_logged_in->add_reservation(new Reservation(user_logged_in, uid, hotel, rdate, rooms));
		else
			throw Not_enough_credit();
		if (user_logged_in->has_enough_reservations())
			default_price_filter = true;
	}
	catch(const Not_enough_room&)
	{
		delete rdate;
		throw;
	}
	catch(const Not_enough_credit&)
	{
		delete rdate;
		throw;
	}
	catch(...)
	{
		throw Bad_request();
	}
}

Reserve_arguments Utrip::read_reservation_args(map<string, string> arguments)
{
	Reserve_arguments args;
	args.hotel_uid = arguments.at(HOTEL_ARG);
	args.type = arguments.at(TYPE_ARG);
	args.quantity = stoi(arguments.at(QUANTITY_ARG));
	args.checkin = stoi(arguments.at(CHECKIN_ARG));
	args.checkout = stoi(arguments.at(CHECKOUT_ARG));
	return args;
}

void Utrip::check_room_type(string type)
{
	if(type != STDROOMTYPE && type != DELUXEROOMTYPE && type != LUXROOMTYPE && type != PREMROOMTYPE)
		throw Bad_request();
}

void Utrip::print_reserves()
{
	check_signed_in();
	user_logged_in->print_reservations(); 
}

Hotel* Utrip::find_hotel(string uid)
{
	for (Hotel& hotel : hotels)
		if (hotel.get_uid() == uid)
			return &hotel;
	throw Not_found();
}

void Utrip::delete_reserve(int uid)
{
	check_signed_in();
	user_logged_in->delete_reserve(uid);
	if (!user_logged_in->has_enough_reservations())
		default_price_filter = false;
	COMMAND_EXECUTED;
}

void Utrip::add_comment(map<string, string> arguments)
{
	check_signed_in();
	string hotel_uid, comment;
	try
	{
		hotel_uid = arguments.at(HOTEL_ARG);
		comment = arguments.at(COMMENT_ARG);
	}
	catch (const out_of_range&)
	{
		throw Bad_request();
	}
	Hotel* hotel = find_hotel(hotel_uid);
	hotel->add_comment(user_logged_in->get_username(), comment);
	COMMAND_EXECUTED;
}

void Utrip::add_rating(map<string, string> arguments)
{
	check_signed_in();
	string hotel_uid;
	double location, cleanliness, staff, facilities, v_for_money, overall;
	try
	{
		hotel_uid = arguments.at(HOTEL_ARG);
		location = stod(arguments.at(LOCATION_ARG));
		cleanliness = stod(arguments.at(CLEANLINESS_ARG));
		staff = stod(arguments.at(STAFF_ARG));
		facilities = stod(arguments.at(FACILITIES_ARG));
		v_for_money = stod(arguments.at(VALUE_FOR_MONEY_ARG));
		overall = stod(arguments.at(OVERALL_ARG));
	}
	catch (const out_of_range&)
	{
		throw Bad_request();
	}
	Hotel* hotel = find_hotel(hotel_uid);
	hotel->add_rating(new Rating(location, cleanliness, staff, facilities, v_for_money, overall, user_logged_in));
	estimated_weights_for_user.clear();
	estimated_weights_calculated = false;
	COMMAND_EXECUTED;
}

void Utrip::show_comments(string hotel_uid)
{
	check_signed_in();
	Hotel* hotel = find_hotel(hotel_uid);
	hotel->show_comments();
}

void Utrip::check_signed_in()
{
	if (!signed_in || user_logged_in == NULL)
		throw Permission_error();
}

void Utrip::show_rating(string hotel_uid)
{
	check_signed_in();
	Hotel* hotel = find_hotel(hotel_uid);
	hotel->show_rating();
}

void Utrip::show_wallet(int count)
{
	check_signed_in();
	user_logged_in->show_wallet(count);
}

void Utrip::set_default_price_filter(string mode)
{
	check_signed_in();
	if (mode == "true")
		default_price_filter = true;
	else if (mode == "false")
		default_price_filter = false;
	else
		throw Bad_request();
	COMMAND_EXECUTED;
}

vector<Hotel*> Utrip::get_printable_hotels()
{
	vector<Hotel*> printable;
	if (default_price_filter)
	{
		for (Hotel*& hotel : filtered_hotels)
			if (user_logged_in->is_hotel_suitable(hotel))
				printable.push_back(hotel);
	}
	else
		return filtered_hotels;
	return printable;
}

void Utrip::sort_hotels(string property_type, bool ascending)
{
	check_signed_in();
	if (property_type == ID_ARG)
	{
		if (ascending)
			sort_func = Sort::ascending_uid_sort;
		else
			sort_func = Sort::descending_uid_sort;
	}
	else if (property_type == NAME_ARG)
	{
		if (ascending)
			sort_func = Sort::ascending_name_sort;
		else
			sort_func = Sort::descending_name_sort;
	}
	else if (property_type == STAR_ARG)
	{
		if (ascending)
			sort_func = Sort::ascending_star_sort;
		else
			sort_func = Sort::descending_star_sort;
	}
	else if (property_type == CITY_ARG)
	{
		if (ascending)
			sort_func = Sort::ascending_city_sort;
		else
			sort_func = Sort::descending_city_sort;
	}
	else if (property_type == STD_ROOM_PRICE_ARG)
	{
		if (ascending)
			sort_func = Sort::ascending_std_room_price_sort;
		else
			sort_func = Sort::descending_std_room_price_sort;
	}
	else if (property_type == DELUXE_ROOM_PRICE_ARG)
	{
		if (ascending)
			sort_func = Sort::ascending_deluxe_room_price_sort;
		else
			sort_func = Sort::descending_deluxe_room_price_sort;
	}
	else if (property_type == LUX_ROOM_PRICE_ARG)
	{
		if (ascending)
			sort_func = Sort::ascending_lux_room_price_sort;
		else
			sort_func = Sort::descending_lux_room_price_sort;
	}
	else if (property_type == PREM_ROOM_PRICE_ARG)
	{
		if (ascending)
			sort_func = Sort::ascending_prem_room_price_sort;
		else
			sort_func = Sort::descending_prem_room_price_sort;
	}
	else if (property_type == AVG_ROOM_PRICE_ARG)
	{
		if (ascending)
			sort_func = Sort::ascending_avg_room_price_sort;
		else
			sort_func = Sort::descending_avg_room_price_sort;
	}
	else if (property_type == RATING_OVERALL_ARG)
	{
		if (ascending)
			sort_func = Sort::ascending_rating_overall_sort;
		else
			sort_func = Sort::descending_rating_overall_sort;
	}
	else if (property_type == RATING_PERSONAL_ARG)
	{
		personal_sort_mod = true;
		if (manual_weights)
			personal_sort = new Personal_sort(user_logged_in, ascending);
		else
		{
			if (!estimated_weights_calculated)
			{
				estimated_weights_calculated = true;
				estimated_weights_for_user = Estimated_rating(user_logged_in, get_all_hotels()).get_estimated_weights();
			}
				personal_sort = new Estimated_weight_sort(user_logged_in, estimated_weights_for_user, ascending);
		}
	}
	else
		throw Bad_request();
	user_sort_type = make_pair(property_type, ascending);
	user_sort = true;
	COMMAND_EXECUTED;
}

void Utrip::sort_hotels(vector<Hotel*>& hotels)
{
	if (!user_sort)
		sort(hotels.begin(), hotels.end(), Sort::ascending_uid_sort);
	else if (personal_sort_mod)
		personal_sort->do_sort(hotels);
	else
		sort(hotels.begin(), hotels.end(), sort_func);
}

void Utrip::load_ratings(string assets_path)
{
	ifstream infile(assets_path);
	string buffer;
	getline(infile, buffer);

	while (getline(infile, buffer))
	{
		vector<string> data = split(buffer, CSV_DELIM);
		Hotel* hotel = find_hotel(data[HOTEL_ID]);
		hotel->add_rating(new Rating(stod(data[RATING_LOCATION]), stod(data[RATING_CLEANLINESS]),
			stod(data[RATING_STAFF]), stod(data[RATING_FACILITIES]), stod(data[RATING_V_FOR_MONEY]),
			stod(data[RATING_OVERALL])));
	}
}

void Utrip::add_manual_weights(Rating* rating)
{
	check_signed_in();
	user_logged_in->set_manual_weights(rating);
	manual_weights = true;
	COMMAND_EXECUTED;
}

void Utrip::show_manual_weights()
{
	check_signed_in();
	Rating* _manual_weights;
	if (manual_weights)
		_manual_weights = user_logged_in->get_manual_weights();
	else
	{
		cout << "active false" << endl;
		return;
	}
	cout << "active true location " << truncate(to_string(_manual_weights->location)) <<
		" cleanliness " << truncate(to_string(_manual_weights->cleanliness)) <<
		" staff " << truncate(to_string(_manual_weights->staff)) <<
		" facilities " << truncate(to_string(_manual_weights->facilities)) <<
		" value_for_money " << truncate(to_string(_manual_weights->value_for_money)) << " \n";
}

void Utrip::show_estimated_weights()
{
	check_signed_in();
	if (!estimated_weights_calculated)
	{
		estimated_weights_calculated = true;
		estimated_weights_for_user = Estimated_rating(user_logged_in, get_all_hotels()).get_estimated_weights();
	}
	if (estimated_weights_for_user.empty())
		throw Insufficient_rating();
	cout << "location " << truncate(to_string(estimated_weights_for_user[0])) <<
		" cleanliness " << truncate(to_string(estimated_weights_for_user[1])) <<
		" staff " << truncate(to_string(estimated_weights_for_user[2])) <<
		" facilities " << truncate(to_string(estimated_weights_for_user[3])) <<
		" value_for_money " << truncate(to_string(estimated_weights_for_user[4])) << " \n";
}

string Utrip::truncate(string num)
{
	string truncated = "";
	unsigned int i;
	for (i = 0; i < num.size() && i < 4; i++)
		truncated += num[i];
	if (i == 1)
		truncated += ".00";
	else if (i == 3)
		truncated += "0";
	return truncated;
}

vector<Hotel*> Utrip::get_all_hotels()
{
	vector<Hotel*> all_hotels;
	for (Hotel& hotel : hotels)
		all_hotels.push_back(&hotel);
	return all_hotels;
}