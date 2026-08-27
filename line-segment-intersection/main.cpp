#include <algorithm>
#include <map>
#include <print>
#include <set>
#include <vector>

#include "primitives.h"


int main()
{
    Point a = { 0, 0 };
    Point b = { 1, 1 };
    std::println("Hello World!");
    std::println("a < b = {}", a < b);

    Segment ab{b, a};
    std::println("({}, {}), ({}, {})", ab.a.x, ab.a.y, ab.b.x, ab.b.y);

    Segment seg1(Point{0.0, 0.0}, Point{4.0, 4.0});
    Segment seg2(Point{0.0, 4.0}, Point{4.0, 0.0});

    if (do_intersect(seg1, seg2))
    {
        std::println("Success: Segment 1 and segment 2 intersect.");
    }

}

void line_segment_intersection(const std::vector<Segment>& segments)
{
    using SegmentTag = size_t;
    std::map<Point, std::set<size_t>> event_queue;

    for (size_t i = 0; auto&& s : segments)
    {
        if (!event_queue.contains(s.a))
        {
            event_queue[s.a] = std::set<SegmentTag>();
        }
        
        if (!event_queue.contains(s.b))
        {
            event_queue[s.b] = std::set<SegmentTag>({ i });
        }
        else
        {
            event_queue[s.b].insert(i);
        }

        ++i;
    }

    std::set<SegmentTag> T; // status

    while (!event_queue.empty())
    {
        auto it = event_queue.begin();
        auto& p = it->first;
        // 5. Handle event
        auto& Up = it->second;

        // 5.2 Find all segments in status T containing p
        std::set<SegmentTag> Lp; // Segments whose lower endpoint is p
        std::set<SegmentTag> Cp; // Segments that contain p in their interior

        for (SegmentTag tag : T)
        {
            const auto& s = segments[tag];
    
            if (s.a == p)
            {
                Lp.insert(tag);
            }

            auto o = orientation(s.a, s.b, p);
            
            if (o == Orientation::Collinear && on_segment(s.a, s.b, p))
            {
                Cp.insert(tag);
            }
        }

        // 5.3 Set union
        std::set<SegmentTag> LCp;
        std::set_union(Lp.begin(), Lp.end(), Cp.begin(), Cp.end(), std::back_insert_iterator(LCp));

        std::set<SegmentTag> LCUp;
        std::set_union(LCp.begin(), LCp.end(), Up.begin(), Up.end(), std::back_insert_iterator(LCUp));

        if (LCUp.size() > 1)
        {
            // p is an intersection, segments are LCUp;
        }

        // 5.4 Delete L(p) U C(p) from T
        for (SegmentTag tag : LCp)
        {
            T.erase(tag);
        }

        // 5.5 Insert the segments in U(p) U C(p) into T
        // The order of the segments in T should correspond to the order in which they are intersected by a sweep line just below p.
        // If there is a horizontal segment, it comes last among all segments containing p.
        // TODO: How to enforce it?
        std::set<SegmentTag> UCp;
        std::set_union(Up.begin(), Up.end(), Cp.begin(), Cp.end(), std::back_insert_iterator(UCp));
        for (SegmentTag tag : UCp)
        {
            T.insert(tag);
        }

        event_queue.erase(it);
    }
}