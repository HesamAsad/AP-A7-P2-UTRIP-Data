#include "Filter.h"
#include "Exceptions.h"
using namespace std;

vector<Hotel*> City_filter::apply(vector<Hotel>& hotels)
{
	vector<Hotel*> filtered;
	for (Hotel& hotel : hotels)
		if (hotel.get_city() == city)
			filtered.push_back(&hotel);
	return filtered;
}

vector<Hotel*> Stars_filter::apply(vector<Hotel>& hotels)
{
	vector<Hotel*> filtered;
	for (Hotel& hotel : hotels)
	{
		int stars = hotel.get_stars();
		if (stars >= min_stars && stars <= max_stars)
			filtered.push_back(&hotel);
	}
	return filtered;
}

vector<Hotel*> Price_filter::apply(vector<Hotel>& hotels)
{
	vector<Hotel*> filtered;
	for (Hotel& hotel : hotels)
	{
		double avg = hotel.get_avg_price();
		if (avg >= min_price && avg <= max_price)
			filtered.push_back(&hotel);
	}
	return filtered;
}

std::vector<Hotel*> Compound_filter::apply(std::vector<Hotel>& hotels)
{
	vector<Hotel*> filtered;
	Reservation_date r_date(checkin, checkout);
	for (Hotel& hotel : hotels)
	{
		try
		{
			vector<Room*> found = hotel.find_available_rooms(quantity, type, r_date);
			if (!found.empty())
				filtered.push_back(&hotel);
		}
		catch (const Not_enough_room&)
		{
			continue;
		}
	}
	return filtered;
}