#include "Reservation_date.h"
using namespace std;

Reservation_date::Reservation_date(int start, int end)
{
	if (start > 30 || start <= 0 || end <= 0 || end > 30 || end < start)
		throw Bad_request();
	start_date = start;
	end_date = end;
	reservation_dates = end - start + 1;
}

bool Reservation_date::dates_overlap(const Reservation_date& r1, const Reservation_date& r2)
{
	if (r2.start_date >= r1.start_date && r2.start_date <= r1.end_date)
		return true;
	if (r2.end_date >= r1.start_date && r2.end_date <= r1.end_date)
		return true;
	return false;
}