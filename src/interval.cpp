#include "interval.h"

const Interval Interval::empty = Interval(INFINITY, -INFINITY);
const Interval Interval::universe = Interval(-INFINITY, INFINITY);
const Interval Interval::unit = Interval(0.f, 1.f);
