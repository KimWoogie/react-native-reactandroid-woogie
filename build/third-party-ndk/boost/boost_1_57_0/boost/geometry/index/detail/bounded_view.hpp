// Boost.Geometry Index
//
// This view makes possible to treat some simple primitives as its bounding geometry
// e.g. box, nsphere, etc.
//
// Copyright (c) 2014 Adam Wulkiewicz, Lodz, Poland.
//
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_INDEX_DETAIL_BOUNDED_VIEW_HPP
#define BOOST_GEOMETRY_INDEX_DETAIL_BOUNDED_VIEW_HPP

namespace boost { namespace geometry {

namespace index { namespace detail {

template <typename Geometry,
          typename BoundingGeometry,
          typename Tag = typename geometry::tag<Geometry>::type,
          typename BoundingTag = typename geometry::tag<BoundingGeometry>::type>
struct bounded_view
{
    BOOST_MPL_ASSERT_MSG(
        (false),
        NOT_IMPLEMENTED_FOR_THOSE_GEOMETRIES,
        (BoundingTag, Tag));
};


// Segment -> Box

template <typename Segment, typename Box>
struct bounded_view<Segment, Box, segment_tag, box_tag>
{
public:
    typedef typename geometry::coordinate_type<Box>::type coordinate_type;

    explicit bounded_view(Segment const& segment)
        : m_segment(segment)
    {}
    
    template <std::size_t Dimension>
    inline coordinate_type get_min() const
    {
        return boost::numeric_cast<coordinate_type>(
                (std::min)( geometry::get<0, Dimension>(m_segment),
                            geometry::get<1, Dimension>(m_segment) ) );
    }

    template <std::size_t Dimension>
    inline coordinate_type get_max() const
    {
        return boost::numeric_cast<coordinate_type>(
                (std::max)( geometry::get<0, Dimension>(m_segment),
                            geometry::get<1, Dimension>(m_segment) ) );
    }

private:
    Segment const& m_segment;
};

// Box -> Box

template <typename BoxIn, typename Box>
struct bounded_view<BoxIn, Box, box_tag, box_tag>
{
public:
    typedef typename geometry::coordinate_type<Box>::type coordinate_type;

    explicit bounded_view(BoxIn const& box)
        : m_box(box)
    {}

    template <std::size_t Dimension>
    inline coordinate_type get_min() const
    {
        return boost::numeric_cast<coordinate_type>(
                geometry::get<min_corner, Dimension>(m_box) );
    }

    template <std::size_t Dimension>
    inline coordinate_type get_max() const
    {
        return boost::numeric_cast<coordinate_type>(
                geometry::get<max_corner, Dimension>(m_box) );
    }

private:
    BoxIn const& m_box;
};

// Point -> Box

template <typename Point, typename Box>
struct bounded_view<Point, Box, point_tag, box_tag>
{
public:
    typedef typename geometry::coordinate_type<Box>::type coordinate_type;

    explicit bounded_view(Point const& point)
        : m_point(point)
    {}

    template <std::size_t Dimension>
    inline coordinate_type get_min() const
    {
        return boost::numeric_cast<coordinate_type>(
                geometry::get<Dimension>(m_point) );
    }

    template <std::size_t Dimension>
    inline coordinate_type get_max() const
    {
        return boost::numeric_cast<coordinate_type>(
                geometry::get<Dimension>(m_point) );
    }

private:
    Point const& m_point;
};

}} // namespace index::detail

// XXX -> Box

#ifndef DOXYGEN_NO_TRAITS_SPECIALIZATIONS
namespace traits
{

template <typename Geometry, typename Box, typename Tag>
struct tag< index::detail::bounded_view<Geometry, Box, Tag, box_tag> >
{
    typedef box_tag type;
};

template <typename Segment, typename Box, typename Tag>
struct point_type< index::detail::bounded_view<Segment, Box, Tag, box_tag> >
{
    typedef typename point_type<Box>::type type;
};

template <typename Segment, typename Box, typename Tag, std::size_t Dimension>
struct indexed_access<index::detail::bounded_view<Segment, Box, Tag, box_tag>,
                      min_corner, Dimension>
{
    typedef index::detail::bounded_view<Segment, Box, Tag, box_tag> box_type;
    typedef typename geometry::coordinate_type<Box>::type coordinate_type;

    static inline coordinate_type get(box_type const& b)
    {
        return b.template get_min<Dimension>();
    }

    //static inline void set(box_type & b, coordinate_type const& value)
    //{
    //    BOOST_ASSERT(false);
    //}
};

template <typename Segment, typename Box, typename Tag, std::size_t Dimension>
struct indexed_access<index::detail::bounded_view<Segment, Box, Tag, box_tag>,
                      max_corner, Dimension>
{
    typedef index::detail::bounded_view<Segment, Box, Tag, box_tag> box_type;
    typedef typename geometry::coordinate_type<Box>::type coordinate_type;

    static inline coordinate_type get(box_type const& b)
    {
        return b.template get_max<Dimension>();
    }

    //static inline void set(box_type & b, coordinate_type const& value)
    //{
    //    BOOST_ASSERT(false);
    //}
};

} // namespace traits
#endif // DOXYGEN_NO_TRAITS_SPECIALIZATIONS

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_INDEX_DETAIL_BOUNDED_VIEW_HPP
