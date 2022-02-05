#include <iostream>
#include "Utrip.h"
#include "Interface.h"
using namespace std;


int main(int argc, char** argv)
{
	string hotel_assets_path = string(argv[1]);
	string rating_assets_path = string(argv[2]);
	Utrip utrip;
	Interface _interface(&utrip);
	utrip.load_hotels(hotel_assets_path);
	utrip.load_ratings(rating_assets_path);
	string command;
	while (getline(cin, command))
	{
		try
		{
			_interface.handle_command(command);
		}
		catch (exception& err)
		{
			cout << err.what() << endl;
		}
	}
	return 0;
}