

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

struct LinearInterpolation
{
    template <typename Iterator, typename Entries>
    static double calc(double x, const Iterator &iter, const Entries &entries)
    {
		// Calculate linear interpolation
		auto it_prev = std::prev(iter);
		
		const double &x0 = (*it_prev)[0];
		const double &x1 = (*iter)[0];
		const double &y0 = (*it_prev)[1];
		const double &y1 = (*iter)[1];

		return (y1 - y0) * (x - x0) / (x1 - x0) + y0;
    }
};

struct Clamp
{
    template <typename Iterator, typename Entries>
    static std::pair<bool,double> calc(double x, const Iterator &iter, const Entries &entries)
    {
		// If "it" is end(), returns the last value
		if (iter == entries.end()) return {true,(*std::prev(entries.end()))[1]};
		
		// It "it" is begin(), returns the first value
		if (iter == entries.begin()) return {true, (*iter)[1]};

        return {false, 0.0};
    }
};

template <typename InterpolationPolicy = LinearInterpolation, typename ExtrapolationPolicy = Clamp>
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

        auto extrapolation = ExtrapolationPolicy::calc(x, it, _entries);
        if (extrapolation.first) return extrapolation.second;

        return InterpolationPolicy::calc(x, it, _entries);
	}

private:
	Entries _entries;

};
