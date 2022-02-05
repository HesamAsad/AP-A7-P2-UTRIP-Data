#ifndef RATING_H
#define RATING_H
#include "User.h"
#include "Exceptions.h"

class Rating
{
public:
	Rating() {}
	Rating(double _location, double _cleanliness, double _staff,
		double _facilities, double v_for_money, double _overall, User* _user = NULL)
	{
		if (_location < 1 || _cleanliness < 1 || _staff < 1 || _facilities < 1 || v_for_money < 1 || _overall < 1
			|| _location > 5 || _cleanliness > 5 || _staff > 5 || _facilities > 5 || v_for_money > 5 || _overall > 5)
			throw Bad_request();
		location = _location; cleanliness = _cleanliness; staff = _staff; user = _user;
		facilities = _facilities; value_for_money = v_for_money; overall = _overall;
	}
	inline User* get_user() { return user; }
	static double get_weighted_avg(Rating* manual_weights, Rating* hotel_avg_rating);
	static double get_weighted_avg(std::vector<double> weights, Rating* hotel_avg_rating);
	double location, cleanliness, staff, facilities, value_for_money, overall;
private:
	User* user;
};

#endif