//#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

//- A helper macro to assist in readability of test functions below.
//
#define ASSERT_A_ADD_B_EQ_C(A, B, C)    \
    static_assert(std::is_same_v<decltype(std::declval<A>() + std::declval<B>()), C>)


//--------------------------------------------------------------------------------------------------
//  The following are several traits types used to exercise the element, engine, and operation
//  type detection meta-functions in the private implementation.  See test function t100() below.
//--------------------------------------------------------------------------------------------------
//
//- This traits type is used to verify that default operations are selected when they are not
//  declared in the operations traits type.
//
struct test_add_op_traits_empty {};


//- This operation traits type has its element/engine/arithmetic nested traits type as ordinary
//  (i.e., non-template) type aliases.
//
//  Suffix "_ord" means "ordinary"
//
struct test_element_add_traits_ord
{
    using element_type = dummy_type;
};

struct test_engine_add_traits_ord
{
    using engine_type = dummy_type;
};

struct test_add_traits_ord
{
    using result_type = dummy_type;
};

struct test_add_op_traits_ord
{
    using addition_element_traits = test_element_add_traits_ord;
    using addition_engine_traits  = test_engine_add_traits_ord;
    using addition_arithmetic_traits         = test_add_traits_ord;
};


//- This operation traits type is analogous to STD_LA::matrix_operation_traits, where its nested
//  traits types for element/engine/arithmetic are alias templates.
//
//  Suffix "_nta" means "nested type alias"
//
template<class T1, class T2>
struct test_element_add_traits_nta
{
    using element_type = dummy_type;
};

template<class OT, class T1, class T2>
struct test_engine_add_traits_nta
{
    using engine_type = dummy_type;
};

template<class OT, class T1, class T2>
struct test_add_traits_nta
{
    using result_type = dummy_type;
};

struct test_add_op_traits_nta
{
    template<class T1, class T2>
    using addition_element_traits = test_element_add_traits_nta<T1, T2>;

    template<class OT, class ET1, class ET2>
    using addition_engine_traits = test_engine_add_traits_nta<OT, ET1, ET2>;

    template<class OT, class OP1, class OP2>
    using addition_arithmetic_traits = test_add_traits_nta<OT, OP1, OP2>;
};


//- This operation traits type has its element/engine/arithmetic traits as nested class templates.
//
//  Suffix "_nct" means "nested class type"
//
struct test_add_op_traits_nct
{
    template<class T1, class T2>
    struct addition_element_traits
    {
        using element_type = dummy_type;
    };

    template<class OT, class ET1, class ET2>
    struct addition_engine_traits
    {
        using engine_type = dummy_type;
    };

    template<class OT, class OP1, class OP2>
    struct addition_arithmetic_traits
    {
        using result_type = dummy_type;
    };
};


//--------------------------------------------------------------------------------------------------
//  This test ensures that the type detection meta-functions are working properly.  It exercises
//  only the detection meta-functions.
//--------------------------------------------------------------------------------------------------
//
void t100()
{
    PRINT_FNAME();

    //- Detect element traits.
    //
    using elem_t = double;

    static_assert(!STD_LA::detail::has_element_add_traits_v<test_add_op_traits_empty, elem_t, elem_t>);
    static_assert(!STD_LA::detail::has_element_add_traits_v<void, elem_t, elem_t>);

    static_assert(STD_LA::detail::has_element_add_traits_v<STD_LA::matrix_operation_traits, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_add_traits_v<test_add_op_traits_ord, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_add_traits_v<test_add_op_traits_nta, elem_t, elem_t>);
    static_assert(STD_LA::detail::has_element_add_traits_v<test_add_op_traits_nct, elem_t, elem_t>);

    //- Detect engine traits.
    //
    using eng_t = STD_LA::dr_matrix_engine<elem_t, std::allocator<elem_t>>;

    static_assert(!STD_LA::detail::has_engine_add_traits_v<test_add_op_traits_empty, eng_t, eng_t>);
    static_assert(!STD_LA::detail::has_engine_add_traits_v<void, eng_t, eng_t>);

    static_assert(STD_LA::detail::has_engine_add_traits_v<STD_LA::matrix_operation_traits, eng_t, eng_t>);
    static_assert(STD_LA::detail::has_engine_add_traits_v<test_add_op_traits_ord, eng_t, eng_t>);
    static_assert(STD_LA::detail::has_engine_add_traits_v<test_add_op_traits_nta, eng_t, eng_t>);
    static_assert(STD_LA::detail::has_engine_add_traits_v<test_add_op_traits_nct, eng_t, eng_t>);

    //- Detect operation traits.
    //
    using opnd_t = STD_LA::dyn_matrix<elem_t>;

    static_assert(!STD_LA::detail::has_add_traits_v<test_add_op_traits_empty, opnd_t, opnd_t>);
    static_assert(!STD_LA::detail::has_add_traits_v<void, opnd_t, opnd_t>);

    static_assert(STD_LA::detail::has_add_traits_v<STD_LA::matrix_operation_traits, opnd_t, opnd_t>);
    static_assert(STD_LA::detail::has_add_traits_v<test_add_op_traits_ord, opnd_t, opnd_t>);
    static_assert(STD_LA::detail::has_add_traits_v<test_add_op_traits_nta, opnd_t, opnd_t>);
    static_assert(STD_LA::detail::has_add_traits_v<test_add_op_traits_nct, opnd_t, opnd_t>);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that addition operations on matrices return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t101()
{
    PRINT_FNAME();

    using fsm_float      = STD_LA::fs_matrix<float, 2, 3>;
    using fsm_double     = STD_LA::fs_matrix<double, 2, 3>;
    using fsm_new_num    = STD_LA::fs_matrix<new_num, 2, 3>;
    using fsm_float_tr   = decltype(std::declval<STD_LA::fs_matrix<float, 3, 2>>().t());
    using fsm_double_tr  = decltype(std::declval<STD_LA::fs_matrix<double, 3, 2>>().t());
    using fsm_new_num_tr = decltype(std::declval<STD_LA::fs_matrix<new_num, 3, 2>>().t());

    using drm_float      = STD_LA::dyn_matrix<float>;
    using drm_double     = STD_LA::dyn_matrix<double>;
    using drm_new_num    = STD_LA::dyn_matrix<new_num>;
    using drm_float_tr   = decltype(std::declval<drm_float>().t());
    using drm_double_tr  = decltype(std::declval<drm_double>().t());
    using drm_new_num_tr = decltype(std::declval<drm_new_num>().t());

    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_float,       fsm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_double,      fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_float_tr,    fsm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_double_tr,   fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_float,       fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_double,      fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_float_tr,    fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_double_tr,   fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_float,       fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_double,      fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_float_tr,    fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_double_tr,   fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_float,       fsm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_double,      fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_float_tr,    fsm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_double_tr,   fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_float,       fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_double,      fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_float_tr,    fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_double_tr,   fsm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_float,       fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_double,      fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_new_num,     fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_float_tr,    fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_double_tr,   fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  fsm_new_num_tr,  fsm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tr,  drm_new_num_tr,  drm_new_num);

    //------
    //
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_float,       drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_float_tr,    drm_float);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_float_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_float,       drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_double,      drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_float_tr,    drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_double_tr,   drm_double);
    ASSERT_A_ADD_B_EQ_C(drm_double_tr,  drm_new_num_tr,  drm_new_num);

    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  fsm_new_num_tr,  drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_float,       drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_double,      drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_new_num,     drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_float_tr,    drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_double_tr,   drm_new_num);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tr,  drm_new_num_tr,  drm_new_num);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that addition operations on vectors return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t102()
{
    PRINT_FNAME();

    using fsv_float      = STD_LA::fs_vector<float, 3>;
    using fsv_double     = STD_LA::fs_vector<double, 3>;
    using fsv_new_num    = STD_LA::fs_vector<new_num, 3>;

    using drv_float      = STD_LA::dyn_vector<float>;
    using drv_double     = STD_LA::dyn_vector<double>;
    using drv_new_num    = STD_LA::dyn_vector<new_num>;

    ASSERT_A_ADD_B_EQ_C(fsv_float,  fsv_float,       fsv_float);
    ASSERT_A_ADD_B_EQ_C(fsv_float,  fsv_double,      fsv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_float,  fsv_new_num,     fsv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_float,  drv_float,       drv_float);
    ASSERT_A_ADD_B_EQ_C(fsv_float,  drv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_float,  drv_new_num,     drv_new_num);

    ASSERT_A_ADD_B_EQ_C(fsv_double,  fsv_float,       fsv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_double,  fsv_double,      fsv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_double,  fsv_new_num,     fsv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_double,  drv_float,       drv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_double,  drv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(fsv_double,  drv_new_num,     drv_new_num);

    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  fsv_float,       fsv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  fsv_double,      fsv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  fsv_new_num,     fsv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  drv_float,       drv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  drv_double,      drv_new_num);
    ASSERT_A_ADD_B_EQ_C(fsv_new_num,  drv_new_num,     drv_new_num);

    //------
    //
    ASSERT_A_ADD_B_EQ_C(drv_float,  fsv_float,       drv_float);
    ASSERT_A_ADD_B_EQ_C(drv_float,  fsv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_float,  fsv_new_num,     drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_float,  drv_float,       drv_float);
    ASSERT_A_ADD_B_EQ_C(drv_float,  drv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_float,  drv_new_num,     drv_new_num);

    ASSERT_A_ADD_B_EQ_C(drv_double,  fsv_float,       drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_double,  fsv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_double,  fsv_new_num,     drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_double,  drv_float,       drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_double,  drv_double,      drv_double);
    ASSERT_A_ADD_B_EQ_C(drv_double,  drv_new_num,     drv_new_num);

    ASSERT_A_ADD_B_EQ_C(drv_new_num,  fsv_float,       drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_new_num,  fsv_double,      drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_new_num,  fsv_new_num,     drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_new_num,  drv_float,       drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_new_num,  drv_double,      drv_new_num);
    ASSERT_A_ADD_B_EQ_C(drv_new_num,  drv_new_num,     drv_new_num);
}


//--------------------------------------------------------------------------------------------------
//  Suffix "_tst" means "test"
//
template<class T1, class T2>
struct element_add_traits_tst;

//- Promote any float/float addition to double.
//
template<>
struct element_add_traits_tst<float, float>
{
    using element_type = double;
};

//------
//
template<class OT, class ET1, class ET2>
struct engine_add_traits_tst;

template<class OT, class T1, size_t R1, size_t C1, class T2, size_t R2, size_t C2>
struct engine_add_traits_tst<OT,
                             fs_matrix_engine_tst<T1, R1, C1>,
                             fs_matrix_engine_tst<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = STD_LA::select_matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class T2, ptrdiff_t R2, ptrdiff_t C2>
struct engine_add_traits_tst<OT,
                             STD_LA::fs_matrix_engine<T1, R1, C1>,
                             STD_LA::fs_matrix_engine<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = STD_LA::select_matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OT, class T1, ptrdiff_t R1, ptrdiff_t C1, class T2, size_t R2, size_t C2>
struct engine_add_traits_tst<OT,
                             STD_LA::fs_matrix_engine<T1, R1, C1>,
                             fs_matrix_engine_tst<T2, R2, C2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = STD_LA::select_matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OT, class T1, size_t R1, size_t C1, class MCT1, class T2, size_t R2, size_t C2, class MCT2>
struct engine_add_traits_tst<OT,
                             STD_LA::matrix_transpose_engine<fs_matrix_engine_tst<T1, R1, C1>, MCT1>,
                             STD_LA::matrix_transpose_engine<fs_matrix_engine_tst<T2, R2, C2>, MCT2>>
{
    static_assert(R1 == R2);
    static_assert(C1 == C2);
    using element_type = STD_LA::select_matrix_addition_element_t<OT, T1, T2>;
    using engine_type  = fs_matrix_engine_tst<element_type, C1, R1>;
};

//------
//
template<class OTR, class OP1, class OP2>
struct addition_traits_tst;

template<class OTR>
struct addition_traits_tst<OTR,
                           STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, OTR>,
                           STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, OTR>>
{
    using op_traits   = OTR;
    using engine_type = STD_LA::select_matrix_addition_engine_t<op_traits,
                                                         fs_matrix_engine_tst<double, 3, 4>,
                                                         fs_matrix_engine_tst<double, 3, 4>>;
    using result_type = STD_LA::matrix<engine_type, op_traits>;

    static result_type  add(STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, OTR> const& m1,
                            STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, OTR> const& m2)
                        {
                            STD_LA::PrintOperandTypes<result_type>("addition_traits_TST", m1, m2);
                            return result_type();
                        }
};

//------
//
struct test_add_op_traits_tst
{
     template<class T1, class T2>
     using addition_element_traits = element_add_traits_tst<T1, T2>;

     template<class OT, class ET1, class ET2>
     using addition_engine_traits = engine_add_traits_tst<OT, ET1, ET2>;

     template<class OT, class OP1, class OP2>
     using addition_arithmetic_traits = addition_traits_tst<OT, OP1, OP2>;
};


void t103()
{
    PRINT_FNAME();

    static_assert(STD_LA::detail::has_element_add_traits_v<test_add_op_traits_tst, float, float>);
    static_assert(!STD_LA::detail::has_element_add_traits_v<test_add_op_traits_tst, float, double>);
    static_assert(!STD_LA::detail::has_element_add_traits_v<test_add_op_traits_tst, double, float>);

    using t00 = STD_LA::detail::element_add_result_t<test_add_op_traits_tst, float, float>;
    PRINT_TYPE(t00);

    using t01 = STD_LA::detail::element_add_result_t<test_add_op_traits_tst, float, double>;
    PRINT_TYPE(t01);
}

void t104()
{
    PRINT_FNAME();

    using fsm_float       = STD_LA::fs_matrix<float, 2, 3>;
    using fsm_float_tst   = STD_LA::matrix<fs_matrix_engine_tst<float, 2, 3>, test_add_op_traits_tst>;
    using fsm_double_tst  = STD_LA::matrix<fs_matrix_engine_tst<double, 2, 3>, test_add_op_traits_tst>;
    using fsm_new_num_tst = STD_LA::matrix<fs_matrix_engine_tst<new_num, 2, 3>, test_add_op_traits_tst>;

    using fsm_float_tst_tr   = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<float, 3, 2>, test_add_op_traits_tst>>().t());
    using fsm_double_tst_tr  = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<double, 3, 2>, test_add_op_traits_tst>>().t());
    using fsm_new_num_tst_tr = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<new_num, 3, 2>, test_add_op_traits_tst>>().t());

    using drm_double_tst  = STD_LA::matrix<STD_LA::dr_matrix_engine<double, std::allocator<double>>, test_add_op_traits_tst>;
    using drm_new_num_tst = STD_LA::matrix<STD_LA::dr_matrix_engine<new_num, std::allocator<new_num>>, test_add_op_traits_tst>;

    using t00 = STD_LA::detail::engine_add_result_t<test_add_op_traits_tst,
                                                    fs_matrix_engine_tst<float, 3, 4>,
                                                    fs_matrix_engine_tst<float, 3, 4>>;
    PRINT_TYPE(t00);

    using t01 = STD_LA::detail::engine_add_result_t<test_add_op_traits_tst,
                                                    STD_LA::fs_matrix_engine<float, 3, 4>,
                                                    fs_matrix_engine_tst<float, 3, 4>>;
    PRINT_TYPE(t01);

    ASSERT_A_ADD_B_EQ_C(fsm_float,      fsm_float,      fsm_float);
    ASSERT_A_ADD_B_EQ_C(fsm_float,      fsm_float_tst,  fsm_double_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tst,  fsm_float,      drm_double_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tst,  fsm_float_tst,  fsm_double_tst);

    ASSERT_A_ADD_B_EQ_C(fsm_float_tst_tr,  fsm_float_tst_tr,  fsm_double_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tst_tr,  fsm_float_tst,     drm_double_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_float_tst,     fsm_float_tst_tr,  drm_double_tst);

    ASSERT_A_ADD_B_EQ_C(fsm_double_tst,   fsm_double_tst,   fsm_double_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tst,   fsm_new_num_tst,  fsm_new_num_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tst,  fsm_double_tst,   fsm_new_num_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tst,  fsm_new_num_tst,  fsm_new_num_tst);

    ASSERT_A_ADD_B_EQ_C(fsm_double_tst_tr,   fsm_double_tst_tr,   fsm_double_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tst_tr,   fsm_new_num_tst_tr,  fsm_new_num_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tst_tr,  fsm_double_tst_tr,   fsm_new_num_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tst_tr,  fsm_new_num_tst_tr,  fsm_new_num_tst);

    ASSERT_A_ADD_B_EQ_C(fsm_double_tst,   drm_double_tst,   drm_double_tst);
    ASSERT_A_ADD_B_EQ_C(drm_double_tst,   fsm_new_num_tst,  drm_new_num_tst);
    ASSERT_A_ADD_B_EQ_C(drm_new_num_tst,  fsm_double_tst,   drm_new_num_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_new_num_tst,  drm_new_num_tst,  drm_new_num_tst);

    using fsm_double_tst_34 = STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, test_add_op_traits_tst>;

    ASSERT_A_ADD_B_EQ_C(fsm_double_tst,     fsm_double_tst,     fsm_double_tst);
    ASSERT_A_ADD_B_EQ_C(fsm_double_tst_34,  fsm_double_tst_34,  fsm_double_tst_34);
}

void
TestGroup10()
{
    PRINT_FNAME();

    t100();
    t101();
    t102();
    t103();
    t104();
}
