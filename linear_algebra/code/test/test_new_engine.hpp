#ifndef TEST_NEW_ENGINE_HPP_DEFINED
#define TEST_NEW_ENGINE_HPP_DEFINED

#include "linear_algebra.hpp"

//==================================================================================================
//  Fixed-size, fixed-capacity matrix engine.
//==================================================================================================
//
template<class T, size_t R, size_t C>
class fs_matrix_engine_tst
{
    static_assert(R >= 1);
    static_assert(C >= 1);

  public:
    using engine_category = STD_LA::writable_matrix_engine_tag;
    using element_type    = T;
    using value_type      = T;
    using reference       = T&;
    using pointer         = T*;
    using const_reference = T const&;
    using const_pointer   = T const*;
    using difference_type = std::size_t;
    using size_type       = std::uint32_t;
    using size_tuple      = std::tuple<size_type, size_type>;

    static constexpr bool   is_column_major = false;
    static constexpr bool   is_dense        = true;
    static constexpr bool   is_rectangular  = true;
    static constexpr bool   is_row_major    = true;

//    using column_view_type    = STD_LA::column_engine<fs_matrix_engine_tst>;
//    using row_view_type       = STD_LA::row_engine<fs_matrix_engine_tst>;
//    using transpose_view_type = STD_LA::transpose_engine<fs_matrix_engine_tst>;

#ifdef LA_USE_MDSPAN
    using span_type       = typename STD_LA::fs_matrix_engine<T, R, C>::span_type;
    using const_span_type = typename STD_LA::fs_matrix_engine<T, R, C>::const_span_type;
#endif

  public:
    constexpr fs_matrix_engine_tst();
    constexpr fs_matrix_engine_tst(fs_matrix_engine_tst&&) = default;
    constexpr fs_matrix_engine_tst(fs_matrix_engine_tst const&) = default;

    constexpr fs_matrix_engine_tst&     operator =(fs_matrix_engine_tst&&) = default;
    constexpr fs_matrix_engine_tst&     operator =(fs_matrix_engine_tst const&) = default;

    constexpr const_reference   operator ()(size_type i, size_type j) const;

    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    constexpr reference     operator ()(size_type i, size_type j);

    constexpr void      assign(fs_matrix_engine_tst const& rhs);
    template<class ET2>
    constexpr void      assign(ET2 const& rhs);

    constexpr void      swap(fs_matrix_engine_tst& rhs) noexcept;
    constexpr void      swap_columns(size_type j1, size_type j2);
    constexpr void      swap_rows(size_type i1, size_type i2);

  private:
    T   ma_elems[R*C];
};

template<class T, size_t R, size_t C> inline
constexpr
fs_matrix_engine_tst<T,R,C>::fs_matrix_engine_tst()
:   ma_elems()
{
    if constexpr (std::is_arithmetic_v<T>)
    {
        for (auto& elem : ma_elems) elem = static_cast<T>(0);
    }
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::const_reference
fs_matrix_engine_tst<T,R,C>::operator ()(size_type i, size_type j) const
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::size_type
fs_matrix_engine_tst<T,R,C>::columns() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::size_type
fs_matrix_engine_tst<T,R,C>::rows() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::size_tuple
fs_matrix_engine_tst<T,R,C>::size() const noexcept
{
    return size_tuple(R, C);
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::size_type
fs_matrix_engine_tst<T,R,C>::column_capacity() const noexcept
{
    return C;
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::size_type
fs_matrix_engine_tst<T,R,C>::row_capacity() const noexcept
{
    return R;
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::size_tuple
fs_matrix_engine_tst<T,R,C>::capacity() const noexcept
{
    return size_tuple(R, C);
}

template<class T, size_t R, size_t C> inline
constexpr typename fs_matrix_engine_tst<T,R,C>::reference
fs_matrix_engine_tst<T,R,C>::operator ()(size_type i, size_type j)
{
    return ma_elems[i*C + j];
}

template<class T, size_t R, size_t C> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::assign(fs_matrix_engine_tst const& rhs)
{
    if (&rhs != this) return;

    for (size_type i = 0;  i < R*C;  ++i)
    {
        ma_elems[i] = rhs.ma_elems[i];
    }
}

template<class T, size_t R, size_t C>
template<class ET2> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::assign(ET2 const& rhs)
{
    using src_size_type = typename ET2::size_type;

    if (rhs.size() != size())
    {
        throw std::runtime_error("invalid size");
    }

    if constexpr(std::is_same_v<size_type, src_size_type>)
    {
        for (size_type i = 0;  i < rows();  ++i)
        {
            for (size_type j = 0;  j < columns();  ++j)
            {
                (*this)(i, j) = rhs(i, j);
            }
        }
    }
    else
    {
        src_size_type  si = 0;
        src_size_type  sj = 0;
        size_type       di = 0;
        size_type       dj = 0;

        for (; di < rows(); ++di, ++si)
        {
            for (; dj < columns(); ++dj, ++sj)
            {
                (*this)(di, dj) = rhs(si, sj);
            }
        }
    }
}

template<class T, size_t R, size_t C> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::swap(fs_matrix_engine_tst& rhs) noexcept
{
    if (&rhs != this)
    {
        for (size_type i = 0;  i < R*C;  ++i)
        {
            STD_LA::detail::la_swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

template<class T, size_t R, size_t C> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::swap_columns(size_type j1, size_type j2)
{
    if (j1 != j2)
    {
        for (size_type i = 0;  i < R;  ++i)
        {
            STD_LA::detail::la_swap(ma_elems[i*C + j1], ma_elems[i*C + j2]);
        }
    }
}

template<class T, size_t R, size_t C> inline
constexpr void
fs_matrix_engine_tst<T,R,C>::swap_rows(size_type i1, size_type i2)
{
    if (i1 != i2)
    {
        for (size_type j = 0;  j < C;  ++j)
        {
            STD_LA::detail::la_swap(ma_elems[i1*C + j], ma_elems[i2*C + j]);
        }
    }
}

template<class T, size_t R, size_t C>
using fs_matrix_tst = STD_LA::matrix<fs_matrix_engine_tst<T, R, C>>;

#endif  //- TEST_NEW_ENGINE_HPP_DEFINED