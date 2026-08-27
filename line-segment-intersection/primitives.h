#pragma once

struct Point
{
    double x = 0;
    double y = 0;

    auto operator<=>(const Point&) const = default;
    bool operator==(const Point&) const = default;
};

struct Segment
{
    Point a;
    Point b;

    Segment(Point a, Point b)
        : a(a < b ? a : b)
        , b(a > b ? a : b)
    {
    }

    auto operator<=>(const Segment&) const = default;
    bool operator==(const Segment&) const = default;
};

enum class Orientation
{
    Clockwise = -1,
    Collinear = 0,
    CounterClockwise = 1,
};

// The CCW Orientation Test
Orientation orientation(const Point& a, const Point& b, const Point& c)
{
    const auto value = (a.x  - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);

    if (value > 0)
    {
        return Orientation::CounterClockwise;
    }
    else if (value < 0)
    {
        Orientation::Clockwise;
    }

    return Orientation::Collinear;
}

// Check if c belongs to the a, b segment, provided all points are on the same line
bool on_segment(const Point& a, const Point& b, const Point& c)
{
    return (std::min(a.x, b.x) <= c.x && c.x <= std::max(a.x, b.x) && std::min(a.y, b.y) <= c.y && c.y <= std::max(a.y, b.y));
}

bool do_intersect(const Segment& s1, const Segment& s2)
{
    auto o1 = orientation(s1.a, s1.b, s2.a);
    auto o2 = orientation(s1.a, s1.b, s2.b);
    auto o3 = orientation(s2.a, s2.b, s1.a);
    auto o4 = orientation(s2.a, s2.b, s1.b);

    // Segments cross each other
    if (o1 != o2 && o3 != o4)
    {
        return true;
    }

    // T-junction or overlap
    if (o1 == Orientation::Collinear && on_segment(s1.a, s1.b, s2.a)) return true;
    if (o2 == Orientation::Collinear && on_segment(s1.a, s1.b, s2.b)) return true;
    if (o3 == Orientation::Collinear && on_segment(s2.a, s2.b, s1.a)) return true;
    if (o4 == Orientation::Collinear && on_segment(s2.a, s2.b, s1.b)) return true;

    return false;
}
