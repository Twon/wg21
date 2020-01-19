//==================================================================================================
//  File:       public_suport.hpp
//
//  Summary:    This header defines a small number of types and functions for
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_PUBLIC_SUPPORT_HPP_DEFINED
#define LINEAR_ALGEBRA_PUBLIC_SUPPORT_HPP_DEFINED

namespace STD_LA {

template<class TT> inline constexpr
bool    is_scalar_engine_tag = TT::value == scalar_engine_tag::value;

template<class TT> inline constexpr
bool    is_vector_engine_tag = TT::value >= readable_vector_engine_tag::value &&
                               TT::value <= resizable_vector_engine_tag::value;

template<class TT> inline constexpr
bool    is_matrix_engine_tag = TT::value >= readable_matrix_engine_tag::value &&
                               TT::value <= resizable_matrix_engine_tag::value;


template<class ET> inline constexpr
bool    is_scalar_engine_v = detail::is_scalar_v<ET>;

template<class ET> inline constexpr
bool    is_vector_engine_v = detail::is_vector_v<ET>;

template<typename ET> inline constexpr
bool    is_matrix_engine_v = detail::is_matrix_v<ET>;

template<class ET> inline constexpr
bool    is_readable_engine_v = detail::is_readable_v<ET>;

template<class ET> inline constexpr
bool    is_writable_engine_v = detail::is_writable_v<ET>;

template<class ET> inline constexpr
bool    is_resizable_engine_v = detail::is_resizable_v<ET>;

template<class ET1, class ET2> inline constexpr
bool    engines_are_similar_v = detail::engines_match_v<ET1, ET2>;

template<class T>
struct scalar_engine
{
    using engine_category = scalar_engine_tag;
};

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_PUBLIC_SUPPORT_HPP_DEFINED
