#ifndef ESTIMATED_WEIGHTS
#define ESTIMATED_WEIGHTS
#include "User.h"
#include "Hotel.h"
#include "Rating.h"
#include <vector>

#define MINIMUM_HOTEL_COUNT 5

class Estimated_rating
{
public:
	Estimated_rating() {}
	Estimated_rating(User* _user, std::vector<Hotel*> _hotels);
	std::vector<double> get_estimated_weights();
private:
	User* user;
	std::vector<Hotel*> all_hotels;
	std::vector<Rating*> user_ratings;
	std::vector<double> weights;
	const double alpha = 1.0;
	const double epsilon = 0.0001;
	double f(std::vector<double> partial_ratings, std::vector<double> weights);
	double E(std::vector<double> partial_ratings, std::vector<double> weights, double overall_rating);
	double partial_diff_E(std::vector<double> partial_ratings, std::vector<double> weights, double overall_rating, int i);
	void update_weights(std::vector<double> weights, std::vector<double> partial_ratings, double overall_rating);
	std::vector<double> get_partial_ratings(Rating* rating);
	inline bool has_enough_ratings() { return user_ratings.size() >= MINIMUM_HOTEL_COUNT; }
	static double fRand(double fMin, double fMax);
};

#endif