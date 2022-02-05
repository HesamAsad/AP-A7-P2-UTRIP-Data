#include "Estimated_weights.h"
#include "Exceptions.h"
#include "Utrip.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

double Estimated_rating::fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

Estimated_rating::Estimated_rating(User* _user, std::vector<Hotel*> _hotels)
	:user(_user), all_hotels(_hotels)
{
	for (Hotel* hotel : all_hotels)
	{
		Rating* hotel_rating = hotel->get_user_rating(user);
		if (hotel_rating)
			user_ratings.push_back(hotel_rating);
	}
}

vector<double> Estimated_rating::get_partial_ratings(Rating* rating)
{
	vector<double> ratings;
	ratings.push_back(rating->location);
	ratings.push_back(rating->cleanliness);
	ratings.push_back(rating->staff);
	ratings.push_back(rating->facilities);
	ratings.push_back(rating->value_for_money);
	return ratings;
}

double Estimated_rating::f(std::vector<double> partial_ratings, std::vector<double> weights)
{
	double weight_rating_sum = 0;
	double weights_sum = 0;
	for (int i = 0; i < 5; i++)
	{
		weight_rating_sum += (partial_ratings[i] * weights[i]);
		weights_sum += weights[i];
	}
	return weight_rating_sum / weights_sum;
}

double Estimated_rating::E(std::vector<double> partial_ratings, std::vector<double> weights, double overall_rating)
{
	return pow(f(partial_ratings, weights) - overall_rating, 2);
}

double Estimated_rating::partial_diff_E(std::vector<double> partial_ratings, std::vector<double> weights, double overall_rating, int i)
{
	double primary = E(partial_ratings, weights, overall_rating);
	weights[i] += epsilon;
	double secondary = E(partial_ratings, weights, overall_rating);
	return (secondary - primary) / epsilon;
}

vector<double> Estimated_rating::get_estimated_weights()
{
	if (user_ratings.size() < MINIMUM_HOTEL_COUNT)
		weights.clear();
	else
	{
		srand(time(NULL));
		for (int i = 0; i < 5; i++)
			weights.push_back(fRand(1., 5.));
		for (int k = 0; k < 1000; k++)
		{
			vector<double> diffs;
			for (int i = 0; i < 5; i++)
				diffs.push_back(0.);
			for (int j = 0; j < user_ratings.size(); j++)
				for (int i = 0; i < 5; i++)
					diffs[i] += partial_diff_E(get_partial_ratings(user_ratings[j]), weights, user_ratings[j]->overall, i);
			for (int i = 0; i < 5; i++)
			{
				weights[i] -= (alpha*diffs[i]);
				if (weights[i] > 5)
					weights[i] = 5.;
				else if (weights[i] < 1)
					weights[i] = 1.;
			}
		}
	}
	return weights;
}