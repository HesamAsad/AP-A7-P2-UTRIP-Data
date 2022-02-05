#ifndef SORT_H
#define SORT_H

#include "Hotel.h"
#include <vector>

class Sort
{
public:
	static bool ascending_uid_sort(Hotel*& h1, Hotel*& h2);
	static bool descending_uid_sort(Hotel*& h1, Hotel*& h2);
	
	static bool ascending_name_sort(Hotel*& h1, Hotel*& h2);
	static bool descending_name_sort(Hotel*& h1, Hotel*& h2);
	
	static bool ascending_city_sort(Hotel*& h1, Hotel*& h2);
	static bool descending_city_sort(Hotel*& h1, Hotel*& h2);
	
	static bool ascending_star_sort(Hotel*& h1, Hotel*& h2);
	static bool descending_star_sort(Hotel*& h1, Hotel*& h2);
	
	static bool ascending_std_room_price_sort(Hotel*& h1, Hotel*& h2);
	static bool descending_std_room_price_sort(Hotel*& h1, Hotel*& h2);
	
	static bool ascending_deluxe_room_price_sort(Hotel*& h1, Hotel*& h2);
	static bool descending_deluxe_room_price_sort(Hotel*& h1, Hotel*& h2);
	
	static bool ascending_lux_room_price_sort(Hotel*& h1, Hotel*& h2);
	static bool descending_lux_room_price_sort(Hotel*& h1, Hotel*& h2);
	
	static bool ascending_prem_room_price_sort(Hotel*& h1, Hotel*& h2);
	static bool descending_prem_room_price_sort(Hotel*& h1, Hotel*& h2);
	
	static bool ascending_avg_room_price_sort(Hotel*& h1, Hotel*& h2);
	static bool descending_avg_room_price_sort(Hotel*& h1, Hotel*& h2);
	
	static bool ascending_rating_overall_sort(Hotel*& h1, Hotel*& h2);
	static bool descending_rating_overall_sort(Hotel*& h1, Hotel*& h2);

	virtual void do_sort(std::vector<Hotel*>& hotels) = 0;
protected:
	User* user;
};

class Personal_sort : public Sort
{
public:
	Personal_sort() {}
	Personal_sort(User* _user, bool _ascending) 
		: ascending(_ascending) { user = _user; }
	void do_sort(std::vector<Hotel*>& hotels);
private:
	bool ascending;
};

class Estimated_weight_sort :public Sort
{
public:
	Estimated_weight_sort() {}
	Estimated_weight_sort(User* _user, std::vector<double> weights, bool _ascending) : estimated_weights(weights), ascending(_ascending)
	{ user = _user; }
	void do_sort(std::vector<Hotel*>& hotels);
private:
	std::vector<double> estimated_weights;
	bool ascending;
};

#endif