#include "Rating.h"
#include <vector>
#include <algorithm>
using namespace std;

double Rating::get_weighted_avg(Rating* manual_weights, Rating* hotel_avg_rating)
{
	double weights = 0, sum = 0;
	weights += (manual_weights->cleanliness + manual_weights->facilities +
		manual_weights->location + manual_weights->staff + manual_weights->value_for_money);
	sum += (manual_weights->cleanliness * hotel_avg_rating->cleanliness);
	sum += (manual_weights->location * hotel_avg_rating->location);
	sum += (manual_weights->staff * hotel_avg_rating->staff);
	sum += (manual_weights->facilities * hotel_avg_rating->facilities);
	sum += (manual_weights->value_for_money * hotel_avg_rating->value_for_money);
	return sum / weights;
}

double Rating::get_weighted_avg(std::vector<double> weights, Rating* hotel_avg_rating)
{
	double weight = 0, sum = 0;
	for_each(weights.begin(), weights.end(), [&](double _weight) { weight += _weight; });
	sum += (weights[0] * hotel_avg_rating->location);
	sum += (weights[1] * hotel_avg_rating->cleanliness);
	sum += (weights[2] * hotel_avg_rating->staff);
	sum += (weights[3] * hotel_avg_rating->facilities);
	sum += (weights[4] * hotel_avg_rating->value_for_money);
	return sum / weight;
}