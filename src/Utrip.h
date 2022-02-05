#ifndef UTRIP_H
#define UTRIP_H
#include "Hotel.h"
#include "User.h"
#include "Filter.h"
#include "Sort.h"
#include "Estimated_weights.h"
#include <string>
#include <vector>
#include <map>

#define COMMAND_EXECUTED std::cout << "OK" << std::endl
#define EMPTY_ERROR std::cout << "Empty" << std::endl
const char CSV_DELIM = ',';

const std::string EMAIL_ARG = "email";
const std::string USERNAME_ARG = "username";
const std::string PASS_ARG = "password";
const std::string TYPE_ARG = "type";
const std::string HOTEL_ARG = "hotel";
const std::string QUANTITY_ARG = "quantity";
const std::string CHECKIN_ARG = "check_in";
const std::string CHECKOUT_ARG = "check_out";
const std::string COMMENT_ARG = "comment";
const std::string LOCATION_ARG = "location";
const std::string NAME_ARG = "name";
const std::string STAR_ARG = "star_rating";
const std::string CLEANLINESS_ARG = "cleanliness";
const std::string CITY_ARG = "city";
const std::string STD_ROOM_PRICE_ARG = "standard_room_price";
const std::string DELUXE_ROOM_PRICE_ARG = "deluxe_room_price";
const std::string LUX_ROOM_PRICE_ARG = "luxury_room_price";
const std::string PREM_ROOM_PRICE_ARG = "premium_room_price";
const std::string AVG_ROOM_PRICE_ARG = "average_room_price";
const std::string STAFF_ARG = "staff";
const std::string FACILITIES_ARG = "facilities";
const std::string VALUE_FOR_MONEY_ARG = "value_for_money";
const std::string OVERALL_ARG = "overall_rating";
const std::string RATING_OVERALL_ARG = "rating_overall";
const std::string RATING_PERSONAL_ARG = "rating_personal";

enum HOTELS_DATA {
	ID, NAME, STARS, OVERVIEW, AMENITIES,
	CITY, LATITUDE, LONGTITUDE, IMAGE_URL,
	STD_ROOMS_COUNT, DELUXE_ROOMS_COUNT,
	LUX_ROOMS_COUNT, PREM_ROOMS_COUNT,
	STD_ROOMS_PRICE, DELUXE_ROOMS_PRICE,
	LUX_ROOMS_PRICE, PREM_ROOMS_PRICE
};

enum RATINGS_DATA {
	HOTEL_ID, RATING_LOCATION, RATING_CLEANLINESS,
	RATING_STAFF, RATING_FACILITIES, RATING_V_FOR_MONEY, RATING_OVERALL
};

typedef struct
{
	std::string hotel_uid, type;
	int quantity, checkin, checkout;
} Reserve_arguments;

class Utrip
{
public:
	Utrip();
	~Utrip();
	static std::vector<std::string> split(const std::string &str, char delim);

	void load_hotels(std::string assets_path);
	void load_ratings(std::string assetes_path);
	std::vector<Room*> read_rooms(Rooms_info& info);
	void signup_user(std::map<std::string, std::string> arguments);
	void login_user(std::map<std::string, std::string> arguments);
	void add_comment(std::map<std::string, std::string> arguments);
	void add_rating(std::map<std::string, std::string> arguments);
	void reserve(std::map<std::string, std::string> arguments);
	void logout_user();
	void add_to_wallet(double amount);
	bool user_exists(std::string email, std::string username);
	void show_hotels();
	void show_hotel(std::string id);
	void city_filter(std::string city);
	void stars_filter(int min, int max);
	void price_filter(int min, int max);
	void compound_filter(std::string type, int quantity, int checkin, int checkout);
	void delete_filter();
	void print_reserves();
	void delete_reserve(int uid);
	void show_comments(std::string hotel_uid);
	void show_rating(std::string hotel_uid);
	void show_wallet(int count);
	void set_default_price_filter(std::string mode);
	void sort_hotels(std::string property_type, bool ascending);
	inline void set_manual_weights(bool mode) { manual_weights = mode; }
	void add_manual_weights(Rating* rating);
	void show_manual_weights();
	void show_estimated_weights();

private:
	User* find_user(std::string email);
	void check_signed_in();
	void check_room_type(std::string type);
	void initialize_filtered_hotels();
	void update_filtered_hotels(Filter* filter);
	void intersect_filters();
	std::vector<Hotel*> give_intersection(std::vector<std::vector<Hotel*>> all, int i = 0);
	std::vector<Hotel*> give_intersection(std::vector<Hotel*> v1, std::vector<Hotel*> v2);
	Hotel* find_hotel(std::string uid);
	bool filter_applied(std::string type);
	Reserve_arguments read_reservation_args(std::map<std::string, std::string> arguments);
	std::vector<Hotel*> get_all_hotels();
	static std::string truncate(std::string number);
	bool signed_in;
	std::vector<Hotel> hotels;
	std::vector<Hotel*> filtered_hotels;
	std::map<Filter*, std::vector<Hotel*>> all_filters;
	std::vector<User*> users;
	User* user_logged_in;
	bool default_price_filter;
	std::vector<Hotel*> get_printable_hotels();
	std::pair<std::string, bool> user_sort_type;
	void sort_hotels(std::vector<Hotel*>& hotels);
	bool user_sort;
	bool(*sort_func)(Hotel*&, Hotel*&);
	Sort* personal_sort;
	bool manual_weights;
	bool personal_sort_mod;
	bool estimated_weights_calculated;
	std::vector<double> estimated_weights_for_user;
};

#endif