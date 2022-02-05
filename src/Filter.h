#ifndef FILTER_H
#define FILTER_H
#include <vector>
#include <string>
#include "Hotel.h"

#define CITY_TYPE "city"
#define STAR_TYPE "star"
#define PRICE_TYPE "price"
#define COMPOUND_TYPE "compound"

class Filter
{
public:
	virtual ~Filter() = default;
	virtual std::vector<Hotel*> apply(std::vector<Hotel>& hotels) = 0;
	virtual std::string get_type() = 0;
};

class City_filter : public Filter
{
public:
	inline City_filter(std::string _city) { city = _city; }
	std::vector<Hotel*> apply(std::vector<Hotel>& hotels);
	inline std::string get_type() { return CITY_TYPE; }
private:
	std::string city;
};

class Stars_filter : public Filter
{
public:
	inline Stars_filter(int min, int max) { min_stars = min; max_stars = max; }
	std::vector<Hotel*> apply(std::vector<Hotel>& hotels);
	inline std::string get_type() { return STAR_TYPE; }
private:
	int min_stars;
	int max_stars;
};

class Price_filter : public Filter
{
public:
	inline Price_filter(int min, int max) { min_price = min; max_price = max; }
	std::vector<Hotel*> apply(std::vector<Hotel>& hotels);
	inline std::string get_type() { return PRICE_TYPE; }
private:
	int min_price;
	int max_price;
};

class Compound_filter : public Filter
{
public:
	Compound_filter(std::string _type, int _quantity, int _checkin, int _checkout)
	{
		type = _type;
		quantity = _quantity;
		checkin = _checkin;
		checkout = _checkout;
	}
	std::vector<Hotel*> apply(std::vector<Hotel>& hotels);
	inline std::string get_type() { return COMPOUND_TYPE; }
private:
	std::string type;
	int quantity;
	int checkin;
	int checkout;
};

#endif