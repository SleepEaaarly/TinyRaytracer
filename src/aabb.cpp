#include "aabb.h"

const aabb aabb::empty = aabb(Interval::empty, Interval::empty, Interval::empty);
const aabb aabb::universe = aabb(Interval::universe, Interval::universe, Interval::universe);