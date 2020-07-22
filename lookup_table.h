

//
// Lookup table for OpenFOAM codeStream or codedFixedValue
//
// Usage:
//   LookupTable<Linear, Clamp> table(
//     {0.0,  300.0},
//     {0.5,  500.0},
//     {1.0,  300.0}
//   );
//   
//   table(0.25); // 400.0
//   table(1.50); // 300.0 (clamp)
//
// C++14 feature (constexpr initializer list) is required.
//

#include <array>
#include <vector>
#include <algorithm>

class LookupTable
{
public:
	typedef std::array<double, 2> Entry;
	typedef std::vector<Entry> Entries;

	LookupTable(Entries entries) : _entries(entries) {}

	double operator()(double x) const
	{
		auto it = std::lower_bound(
			_entries.begin(),
			_entries.end(),
			Entry{x, -1.0},
			[](const Entry& a, const Entry& b) { return a[0] < b[0]; }
		);

		// If "it" is end(), returns the last value
		if (it == _entries.end()) return (*std::prev(_entries.end()))[1];
		
		// It "it" is begin(), returns the first value
		if (it == _entries.begin()) return (*it)[1];

		// Calculate linear interpolation
		auto it_prev = std::prev(it);
		
		const double &x0 = (*it_prev)[0];
		const double &x1 = (*it)[0];
		const double &y0 = (*it_prev)[1];
		const double &y1 = (*it)[1];

		return (y1 - y0) * (x - x0) / (x1 - x0) + y0;
	}

private:
	Entries _entries;

};
