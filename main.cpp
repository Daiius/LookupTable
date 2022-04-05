

#include <iostream>

#include "lookup_table.h"



int main()
{
	std::cout << "Lookup table test" << std::endl;
	LookupTable<> lut({
		{0.0, 5.0},
		{1.0, 4.0},
		{2.0, 3.0},
		{3.0, 2.0},
		{4.0, 1.0},
		{5.0, 0.0}
	});

	std::cout << lut(0.0) << std::endl;
	std::cout << lut(5.0) << std::endl;
	std::cout << lut(4.5) << std::endl;

	return 0;
}

