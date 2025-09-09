#ifndef INTERVAL_H	
#define INTERVAL_H

class interval {
public:
	double min, max;

	//Defaults to +inf/-inf
	interval() : min(+infinity), max(-infinity){}

	interval(double min, double max): min(min), max(max){}

	//length of interval
	double size() const {
		return max - min;
	}
	// x is inside (inclusive)
	bool contains(double x) const {
		return min <= x && x <= max;
	}
	// x is inside (exculusive)
	bool surrounds(double x) const {
		return min < x && x < max;
	}
	//clamp x to min < x < max 
	double clamp(double x) const {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
#endif // !INTERVAL_H
