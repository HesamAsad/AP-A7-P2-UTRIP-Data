#ifndef RESERVATION_DATE_H
#define RESERVATION_DATE_H
#include "Exceptions.h"

class User;
class Reservation_date
{
public:
	inline Reservation_date() { start_date = end_date = reservation_dates = 0; }
	Reservation_date(int start, int end);
	static bool dates_overlap(const Reservation_date& r1, const Reservation_date& r2);
	inline int get_checkin() { return start_date; }
	inline int get_checkout() { return end_date; }
	inline int get_days() { return reservation_dates; }
private:
	int start_date;
	int end_date;
	int reservation_dates;
};

#endif