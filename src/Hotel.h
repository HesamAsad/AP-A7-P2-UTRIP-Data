#ifndef HOTELS_H
#define HOTELS_H
#include "Rooms.h"
#include "Rating.h"
#include <vector>
#include <string>

#define NO_RATING "No Rating"

typedef std::string comment;
typedef std::string user_name;

class Hotel
{
public:
	Hotel(std::string _uid, std::string _name, int _stars, std::string _review,
		std::string _amenities, std::string _city, double _latitude, double _longtitude,
		std::string _image_url, Rooms_info _rooms_info, std::vector<Room*> _rooms);
	~Hotel();

	static int count_non_zero_rooms(Rooms_info rooms_info);

	inline std::string get_uid() { return uid; }
	inline std::string get_name() { return name; }
	inline std::string get_city() { return city; }
	inline int get_stars() { return stars; }
	inline double get_avg_price() { return average_price; }
	inline int get_std_room_price() { return rooms_info.std_price; }
	inline int get_deluxe_room_price() { return rooms_info.deluxe_price; }
	inline int get_lux_room_price() { return rooms_info.lux_price; }
	inline int get_prem_room_price() { return rooms_info.prem_price; }
	double get_overall_rating();
	std::vector<Room*> get_room_by_type(std::string type);
	std::vector<Room*> find_available_rooms(int quantity, std::string type, const Reservation_date& rdate);
	void print_complete();
	void print();
	void clear();
	void add_comment(std::string user_name, std::string comment);
	void show_comments();
	void show_rating();
	void add_rating(Rating* rating);
	double get_personal_rating(User* user);
	Rating* get_user_rating(User* user);
	double get_estimated_rating(User* user, std::vector<double> estimated_weights);
private:
	std::string uid;
	std::string name;
	int stars;
	std::string review;
	std::string amenities;
	std::string city;
	double latitude;
	double longtitude;
	std::string image_url;	
	Rooms_info rooms_info;
	int total_rooms;
	int non_zero_rooms;
	double total_price;
	double average_price;
	std::vector<Room*> rooms;
	std::vector<Rating*> ratings;
	std::vector<std::pair<user_name, comment>> comments;
	Rating find_average_rating();
};

#endif