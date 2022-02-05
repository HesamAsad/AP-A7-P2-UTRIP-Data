#include "User.h"
#include "Reservation.h"
#include "Hotel.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
using namespace std;

void User::add_reservation(Reservation* reservation)
{
	double price = (double)reservation->get_price();
	wallet.push_back(-1 * price);
	reservations.push_back(reservation);
	reservation_uids++;
	if (has_enough_reservations())
	{
		set_reservations_prices_avg();
		set_std_deviation();
	}
	reservation->print_success();
}

void User::print_reservations()
{
	if (reservations.empty())
		cout << EMPTY_MESSAGE << endl;
	else
		for (int i = reservations.size() - 1; i >= 0; i--)
			reservations[i]->print();
}

double User::all_money()
{
	double sum = 0;
	for (double transaction : wallet)
		sum += transaction;
	return sum;
}

bool User::has_money(int amount)
{
	return amount <= all_money();
}

void User::delete_reserve(int uid)
{
	vector<Reservation*> updated;
	for (Reservation*& reserve : reservations)
		if (reserve->get_uid() != uid)
			updated.push_back(reserve);
		else
		{
			wallet.push_back(double(reserve->get_price()) / 2);
			reserve->clean();
			delete reserve;
		}
	if (reservations.size() == updated.size())
		throw Not_found();
	else
		reservations = updated;
	if (has_enough_reservations())
	{
		set_reservations_prices_avg();
		set_std_deviation();
	}
}

User::~User()
{
	for(Reservation*& reserve : reservations)
		delete reserve;
	if (manual_weights != NULL)
		delete manual_weights;
}

void User::show_wallet(int count)
{
	for (int i = 0; i < count; i++)
	{
		double stage_money = calculate_money(i);
		if (stage_money == -1)
		{
			cout << 0 << endl;
			break;
		}
		cout << int(round(stage_money)) << endl;
	}
}

double User::calculate_money(int stage)
{
	int wallet_size = wallet.size();
	if (stage >= wallet_size)
		return -1;
	double sum = 0;
	for (int i = 0; i < wallet_size - stage; i++)
		sum += wallet[i];
	return sum;
}

bool User::is_hotel_suitable(Hotel* hotel)
{
	return abs(hotel->get_avg_price() - sample_avg) <= (2 * sample_std_deviation);
}

double User::set_reservations_prices_avg()
{
	double price_sum = 0, count = 0;
	for (auto reservation : reservations)
	{
		price_sum += (reservation->get_pure_price());
		count += reservation->get_rooms_count();
	}
	sample_avg = price_sum / count;
	return sample_avg;
}

double User::set_std_deviation()
{
	double sum = 0;
	int count = 0;
	for_each(reservations.begin(), reservations.end(),
		[&](Reservation* r) { sum += r->get_rooms_count() * pow(r->get_pure_price() / r->get_rooms_count() - sample_avg, 2); });
	for_each(reservations.begin(), reservations.end(), [&](Reservation* r) {count += r->get_rooms_count(); });
	sample_std_deviation = sqrt(sum / (count - 1));
	return sample_std_deviation;
}

void User::set_manual_weights(Rating* rating)
{
	if (rating != NULL)
	{
		delete manual_weights;
		manual_weights = rating;
		return;
	}
	manual_weights = rating;
}