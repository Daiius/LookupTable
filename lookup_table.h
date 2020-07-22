

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

#include <utility>
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

		// If it is end(), returns second of the last entry.
		// Otherwize returns second of the lower bound entry.
		return (it == _entries.end()) ? (*std::prev(_entries.end()))[1] : (*it)[1];

	}

private:
	Entries _entries;

};
