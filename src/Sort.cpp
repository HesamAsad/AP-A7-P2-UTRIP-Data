#include "Sort.h"
#include <algorithm>
using namespace std;

bool Sort::ascending_uid_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_uid() < h2->get_uid();
}

bool Sort::descending_uid_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_uid() > h2->get_uid();
}

bool Sort::ascending_name_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_name() == h2->get_name() ?
		ascending_uid_sort(h1, h2) :
		h1->get_name() < h2->get_name();
}

bool Sort::descending_name_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_name() == h2->get_name() ?
		ascending_uid_sort(h1, h2) :
		h1->get_name() > h2->get_name();
}

bool Sort::ascending_city_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_city() == h2->get_city() ?
		ascending_uid_sort(h1, h2) :
		h1->get_city() < h2->get_city();
}

bool Sort::descending_city_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_city() == h2->get_city() ?
		ascending_uid_sort(h1, h2) :
		h1->get_city() > h2->get_city();
}

bool Sort::ascending_star_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_stars() == h2->get_stars() ?
		ascending_uid_sort(h1, h2) :
		h1->get_stars() < h2->get_stars();
}

bool Sort::descending_star_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_stars() == h2->get_stars() ?
		ascending_uid_sort(h1, h2) :
		h1->get_stars() > h2->get_stars();
}

bool Sort::ascending_std_room_price_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_std_room_price() == h2->get_std_room_price() ? 
		ascending_uid_sort(h1, h2) : 
		h1->get_std_room_price() < h2->get_std_room_price();
}

bool Sort::descending_std_room_price_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_std_room_price() == h2->get_std_room_price() ? 
		ascending_uid_sort(h1, h2) : 
		h1->get_std_room_price() > h2->get_std_room_price();
}

bool Sort::ascending_deluxe_room_price_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_deluxe_room_price() == h2->get_deluxe_room_price() ? 
		ascending_uid_sort(h1, h2) : 
		h1->get_deluxe_room_price() < h2->get_deluxe_room_price();
}

bool Sort::descending_deluxe_room_price_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_deluxe_room_price() == h2->get_deluxe_room_price() ?
		ascending_uid_sort(h1, h2) : 
		h1->get_deluxe_room_price() > h2->get_deluxe_room_price();
}

bool Sort::ascending_lux_room_price_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_lux_room_price() == h2->get_lux_room_price() ?
		ascending_uid_sort(h1, h2) :
		h1->get_lux_room_price() < h2->get_lux_room_price();
}

bool Sort::descending_lux_room_price_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_lux_room_price() == h2->get_lux_room_price() ?
		ascending_uid_sort(h1, h2) :
		h1->get_lux_room_price() > h2->get_lux_room_price();
}

bool Sort::ascending_prem_room_price_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_prem_room_price() == h2->get_prem_room_price() ?
		ascending_uid_sort(h1, h2) : 
		h1->get_prem_room_price() < h2->get_prem_room_price();
}

bool Sort::descending_prem_room_price_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_prem_room_price() == h2->get_prem_room_price() ?
		ascending_uid_sort(h1, h2) :
		h1->get_prem_room_price() > h2->get_prem_room_price();
}

bool Sort::ascending_avg_room_price_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_avg_price() == h2->get_avg_price() ?
		ascending_uid_sort(h1, h2) : 
		h1->get_avg_price() < h2->get_avg_price();
}

bool Sort::descending_avg_room_price_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_avg_price() == h2->get_avg_price() ?
		ascending_uid_sort(h1, h2) : 
		h1->get_avg_price() > h2->get_avg_price();
}

bool Sort::ascending_rating_overall_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_overall_rating() == h2->get_overall_rating() ?
		ascending_uid_sort(h1, h2) :
		h1->get_overall_rating() < h2->get_overall_rating();
}

bool Sort::descending_rating_overall_sort(Hotel*& h1, Hotel*& h2)
{
	return h1->get_overall_rating() == h2->get_overall_rating() ?
		ascending_uid_sort(h1, h2) :
		h1->get_overall_rating() > h2->get_overall_rating();
}

void Personal_sort::do_sort(vector<Hotel*>& hotels)
{
	if (ascending)
		sort(hotels.begin(), hotels.end(), [&](Hotel*& h1, Hotel*& h2)
	{
		double h1_personal_rating = h1->get_personal_rating(user);
		double h2_personal_rating = h2->get_personal_rating(user);

		return h1_personal_rating == h2_personal_rating ?
			ascending_uid_sort(h1, h2) :
			h1_personal_rating < h2_personal_rating;
	});

	else
		sort(hotels.begin(), hotels.end(), [&](Hotel*& h1, Hotel*& h2)
	{
		double h1_personal_rating = h1->get_personal_rating(user);
		double h2_personal_rating = h2->get_personal_rating(user);

		return h1_personal_rating == h2_personal_rating ?
			ascending_uid_sort(h1, h2) :
			h1_personal_rating > h2_personal_rating;
	});
}

void Estimated_weight_sort::do_sort(std::vector<Hotel*>& hotels)
{
	if (estimated_weights.empty())
		throw Insufficient_rating();
	if (ascending)
		sort(hotels.begin(), hotels.end(), [&](Hotel*& h1, Hotel*& h2)
		{
			double h1_estimated_rating = h1->get_estimated_rating(user, estimated_weights);
			double h2_estimated_rating = h2->get_estimated_rating(user, estimated_weights);
			
			return h1_estimated_rating == h2_estimated_rating ?
				ascending_uid_sort(h1, h2) :
				h1_estimated_rating < h2_estimated_rating;
		});
	else
		sort(hotels.begin(), hotels.end(), [&](Hotel*& h1, Hotel*& h2)
		{
		double h1_estimated_rating = h1->get_estimated_rating(user, estimated_weights);
		double h2_estimated_rating = h2->get_estimated_rating(user, estimated_weights);
		
		return h1_estimated_rating == h2_estimated_rating ?
			ascending_uid_sort(h1, h2) :
			h1_estimated_rating > h2_estimated_rating;
		});
}