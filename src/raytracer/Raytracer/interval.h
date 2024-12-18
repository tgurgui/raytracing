#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

//==============================================================================================
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

class interval {
  public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    interval(double min, double max) : min(min), max(max) {}

    interval(const interval& a, const interval& b) {
        // Create the interval tightly enclosing the two input intervals.
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    [[nodiscard]] double size() const {
        return max - min;
    }

    [[nodiscard]] bool contains(double x) const {
        return min <= x && x <= max;
    }

    [[nodiscard]] bool surrounds(double x) const {
        return min < x && x < max;
    }

    [[nodiscard]] double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    [[nodiscard]] interval expand(double delta) const {
        auto padding = delta/2;
        return interval(min - padding, max + padding);
    }


    static const interval empty, universe;
};

#endif
