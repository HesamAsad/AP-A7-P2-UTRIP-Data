#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

#define EMPTY_MESSAGE "Empty"

class Reservation;
class Hotel;
class Rating;

class User
{
public:
	User(std::string _username, std::string _email, std::string _password)
	{
		username = _username; email = _email; password = _password;
		reservation_uids = 1; manual_weights = NULL;
	}
	~User();
	inline std::string get_email() { return email; }
	inline std::string get_username() { return username; }
	inline bool is_password_true(std::string pass) { return password == pass; }
	inline int get_reservation_uid() { return reservation_uids; }
	inline bool has_enough_reservations() { return reservations.size() >= 10; }
	void add_to_wallet(double amount) { wallet.push_back(amount); }
	void print_reservations();
	bool has_money(int amount);
	void add_reservation(Reservation* reservation);
	void delete_reserve(int uid);
	void show_wallet(int count);
	bool is_hotel_suitable(Hotel* hotel);
	inline double get_reservations_avg() { return sample_avg; }
	inline double get_std_deviation() { return sample_std_deviation; }
	void set_manual_weights(Rating* _manual_weights);
	inline Rating* get_manual_weights() { return manual_weights; }
private:
	std::string username;
	std::string email;
	std::string password;
	std::vector<double> wallet;
	std::vector<Reservation*> reservations;
	double all_money();
	double calculate_money(int stage);
	int reservation_uids;
	double set_reservations_prices_avg();
	double set_std_deviation();
	double sample_avg;
	double sample_std_deviation;
	Rating* manual_weights;
};

#endif