//#define ENABLE_TEST_PRINTING
#include "test_common.hpp"

#ifdef LA_NEGATION_AS_VIEW

void
TestGroup30()
{
    PRINT_FNAME();
}


#else

//- A helper macro to assist in readability of test functions below.
//
#define ASSERT_NEG_A_EQ_B(A, B)     \
    static_assert(std::is_same_v<decltype(-std::declval<A>()), B>)


//--------------------------------------------------------------------------------------------------
//  The following are several traits types used to exercise the element, engine, and operation
//  type detection meta-functions in the private implementation.  See test function t300() below.
//--------------------------------------------------------------------------------------------------
//
//- This traits type is used to verify that default operations are selected when they are not
//  declared in the operations traits type.
//
struct test_neg_op_traits_empty {};


//- This operation traits type has its element/engine/arithmetic nested traits type as ordinary
//  (i.e., non-template) type aliases.
//
//  Suffix "_ord" means "ordinary"
//
struct test_element_neg_traits_ord
{
    using element_type = dummy_type;
};

struct test_engine_neg_traits_ord
{
    using engine_type = dummy_type;
};

struct test_neg_traits_ord
{
    using result_type = dummy_type;
};

struct test_neg_op_traits_ord
{
    using negation_element_traits = test_element_neg_traits_ord;
    using negation_engine_traits  = test_engine_neg_traits_ord;
    using negation_arithmetic_traits         = test_neg_traits_ord;
};


//- This operation traits type is analogous to STD_LA::matrix_operation_traits, where its nested
//  traits types for element/engine/arithmetic are alias templates.
//
//  Suffix "_nta" means "nested type alias"
//
template<class T1>
struct test_element_neg_traits_nta
{
    using element_type = dummy_type;
};

template<class OT, class ET1>
struct test_engine_neg_traits_nta
{
    using engine_type = dummy_type;
};

template<class OT, class OP1>
struct test_neg_traits_nta
{
    using result_type = dummy_type;
};

struct test_neg_op_traits_nta
{
    template<class T1>
    using negation_element_traits = test_element_neg_traits_nta<T1>;

    template<class OT, class ET1>
    using negation_engine_traits = test_engine_neg_traits_nta<OT, ET1>;

    template<class OT, class OP1>
    using negation_arithmetic_traits = test_neg_traits_nta<OT, OP1>;
};


//- This operation traits type has its element/engine/arithmetic traits as nested class templates.
//
//  Suffix "_nct" means "nested class type"
//
struct test_neg_op_traits_nct
{
    template<class T1>
    struct negation_element_traits
    {
        using element_type = dummy_type;
    };

    template<class OT, class ET1>
    struct negation_engine_traits
    {
        using engine_type = dummy_type;
    };

    template<class OT, class OP1>
    struct negation_arithmetic_traits
    {
        using result_type = dummy_type;
    };
};


//--------------------------------------------------------------------------------------------------
//  This test ensures that the type detection meta-functions are working properly.  It exercises
//  only the detection meta-functions.
//--------------------------------------------------------------------------------------------------
//
void t300()
{
    PRINT_FNAME();

    //- Detect element traits.
    //
    using elem_t = double;

    static_assert(!STD_LA::detail::has_element_neg_traits_v<test_neg_op_traits_empty, elem_t>);
    static_assert(!STD_LA::detail::has_element_neg_traits_v<void, elem_t>);

    static_assert(STD_LA::detail::has_element_neg_traits_v<STD_LA::matrix_operation_traits, elem_t>);
    static_assert(STD_LA::detail::has_element_neg_traits_v<test_neg_op_traits_ord, elem_t>);
    static_assert(STD_LA::detail::has_element_neg_traits_v<test_neg_op_traits_nta, elem_t>);
    static_assert(STD_LA::detail::has_element_neg_traits_v<test_neg_op_traits_nct, elem_t>);

    //- Detect engine traits.
    //
    using eng_t = STD_LA::dr_matrix_engine<elem_t, std::allocator<elem_t>>;

    static_assert(!STD_LA::detail::has_engine_neg_traits_v<test_neg_op_traits_empty, eng_t>);
    static_assert(!STD_LA::detail::has_engine_neg_traits_v<void, eng_t>);

    static_assert(STD_LA::detail::has_engine_neg_traits_v<STD_LA::matrix_operation_traits, eng_t>);
    static_assert(STD_LA::detail::has_engine_neg_traits_v<test_neg_op_traits_ord, eng_t>);
    static_assert(STD_LA::detail::has_engine_neg_traits_v<test_neg_op_traits_nta, eng_t>);
    static_assert(STD_LA::detail::has_engine_neg_traits_v<test_neg_op_traits_nct, eng_t>);

    //- Detect operation traits.
    //
    using opnd_t = STD_LA::dyn_matrix<elem_t>;

    static_assert(!STD_LA::detail::has_neg_traits_v<test_neg_op_traits_empty, opnd_t>);
    static_assert(!STD_LA::detail::has_neg_traits_v<void, opnd_t>);

    static_assert(STD_LA::detail::has_neg_traits_v<STD_LA::matrix_operation_traits, opnd_t>);
    static_assert(STD_LA::detail::has_neg_traits_v<test_neg_op_traits_ord, opnd_t>);
    static_assert(STD_LA::detail::has_neg_traits_v<test_neg_op_traits_nta, opnd_t>);
    static_assert(STD_LA::detail::has_neg_traits_v<test_neg_op_traits_nct, opnd_t>);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that addition operations on matrices return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t301()
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

    ASSERT_NEG_A_EQ_B(fsm_float,      fsm_float);
    ASSERT_NEG_A_EQ_B(fsm_double,     fsm_double);
    ASSERT_NEG_A_EQ_B(fsm_new_num,    fsm_new_num);
    ASSERT_NEG_A_EQ_B(fsm_float_tr,   fsm_float);
    ASSERT_NEG_A_EQ_B(fsm_double_tr,  fsm_double);
    ASSERT_NEG_A_EQ_B(fsm_new_num_tr, fsm_new_num);

    //------
    //
    ASSERT_NEG_A_EQ_B(drm_float,      drm_float);
    ASSERT_NEG_A_EQ_B(drm_double,     drm_double);
    ASSERT_NEG_A_EQ_B(drm_new_num,    drm_new_num);
    ASSERT_NEG_A_EQ_B(drm_float_tr,   drm_float);
    ASSERT_NEG_A_EQ_B(drm_double_tr,  drm_double);
    ASSERT_NEG_A_EQ_B(drm_new_num_tr, drm_new_num);
}

//--------------------------------------------------------------------------------------------------
//  This test verifies that addition operations on vectors return the correct result type.
//--------------------------------------------------------------------------------------------------
//
void t302()
{
    PRINT_FNAME();

    using fsv_float      = STD_LA::fs_vector<float, 3>;
    using fsv_double     = STD_LA::fs_vector<double, 3>;
    using fsv_new_num    = STD_LA::fs_vector<new_num, 3>;

    using drv_float      = STD_LA::dyn_vector<float>;
    using drv_double     = STD_LA::dyn_vector<double>;
    using drv_new_num    = STD_LA::dyn_vector<new_num>;

    ASSERT_NEG_A_EQ_B(fsv_float,    fsv_float);
    ASSERT_NEG_A_EQ_B(fsv_double,   fsv_double);
    ASSERT_NEG_A_EQ_B(fsv_new_num,  fsv_new_num);

    ASSERT_NEG_A_EQ_B(drv_float,    drv_float);
    ASSERT_NEG_A_EQ_B(drv_double,   drv_double);
    ASSERT_NEG_A_EQ_B(drv_new_num,  drv_new_num);
}


//--------------------------------------------------------------------------------------------------
//  Suffix "_tst" means "test"
//
template<class T1>
struct element_neg_traits_tst;

//- Promote any float/float addition to double.
//
template<>
struct element_neg_traits_tst<float>
{
    using element_type = double;
};

//------
//
template<class OT, class ER1>
struct engine_neg_traits_tst;

template<class OT, class T1, size_t R1, size_t C1>
struct engine_neg_traits_tst<OT, fs_matrix_engine_tst<T1, R1, C1>>
{
    using element_type = STD_LA::select_matrix_negation_element_t<OT, T1>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OT, class T1, size_t R1, size_t C1>
struct engine_neg_traits_tst<OT, STD_LA::fs_matrix_engine<T1, R1, C1>>
{
    using element_type = STD_LA::select_matrix_negation_element_t<OT, T1>;
    using engine_type  = fs_matrix_engine_tst<element_type, R1, C1>;
};

template<class OT, class T1, size_t R1, size_t C1, class MCT1>
struct engine_neg_traits_tst<OT, STD_LA::matrix_transpose_engine<fs_matrix_engine_tst<T1, R1, C1>, MCT1>>
{
    using element_type = STD_LA::select_matrix_negation_element_t<OT, T1>;
    using engine_type  = fs_matrix_engine_tst<element_type, C1, R1>;
};

//------
//
template<class OTR, class OP1>
struct negation_traits_tst;

template<class OTR>
struct negation_traits_tst<OTR, STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, OTR>>
{
    using op_traits   = OTR;
    using engine_type = STD_LA::select_matrix_negation_engine_t<op_traits, fs_matrix_engine_tst<double, 3, 4>>;
    using result_type = STD_LA::matrix<engine_type, op_traits>;

    static result_type  negate(STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, OTR> const& m1)
                        {
                            STD_LA::PrintOperandTypes<result_type>("negation_traits_TST", m1);
                            return result_type();
                        }
};

//------
//
struct test_neg_op_traits_tst
{
     template<class T1>
     using negation_element_traits = element_neg_traits_tst<T1>;

     template<class OT, class ET1>
     using negation_engine_traits = engine_neg_traits_tst<OT, ET1>;

     template<class OT, class OP1>
     using negation_arithmetic_traits = negation_traits_tst<OT, OP1>;
};


void t303()
{
    PRINT_FNAME();

    static_assert(STD_LA::detail::has_element_neg_traits_v<test_neg_op_traits_tst, float>);
    static_assert(!STD_LA::detail::has_element_neg_traits_v<test_neg_op_traits_tst, double>);

    using t00 = STD_LA::detail::element_neg_result_t<test_neg_op_traits_tst, float>;
    PRINT_TYPE(t00);

    using t01 = STD_LA::detail::element_neg_result_t<test_neg_op_traits_tst, double>;
    PRINT_TYPE(t01);
}

void t304()
{
    PRINT_FNAME();

    using fsm_float       = STD_LA::fs_matrix<float, 2, 3>;
    using fsm_float_tst   = STD_LA::matrix<fs_matrix_engine_tst<float, 2, 3>, test_neg_op_traits_tst>;
    using fsm_double_tst  = STD_LA::matrix<fs_matrix_engine_tst<double, 2, 3>, test_neg_op_traits_tst>;
    using fsm_new_num_tst = STD_LA::matrix<fs_matrix_engine_tst<new_num, 2, 3>, test_neg_op_traits_tst>;

    using fsm_float_tst_tr   = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<float, 3, 2>, test_neg_op_traits_tst>>().t());
    using fsm_double_tst_tr  = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<double, 3, 2>, test_neg_op_traits_tst>>().t());
    using fsm_new_num_tst_tr = decltype(std::declval<STD_LA::matrix<fs_matrix_engine_tst<new_num, 3, 2>, test_neg_op_traits_tst>>().t());

    using drm_double_tst  = STD_LA::matrix<STD_LA::dr_matrix_engine<double, std::allocator<double>>, test_neg_op_traits_tst>;
    using drm_new_num_tst = STD_LA::matrix<STD_LA::dr_matrix_engine<new_num, std::allocator<new_num>>, test_neg_op_traits_tst>;

    using t00 = STD_LA::detail::engine_neg_result_t<test_neg_op_traits_tst, fs_matrix_engine_tst<float, 3, 4>>;
    PRINT_TYPE(t00);

    using t01 = STD_LA::detail::engine_neg_result_t<test_neg_op_traits_tst, STD_LA::fs_matrix_engine<new_num, 3, 4>>;
    PRINT_TYPE(t01);

    ASSERT_NEG_A_EQ_B(fsm_float,        fsm_float);
    ASSERT_NEG_A_EQ_B(fsm_float_tst,    fsm_double_tst);
    ASSERT_NEG_A_EQ_B(fsm_double_tst,   fsm_double_tst);
    ASSERT_NEG_A_EQ_B(fsm_new_num_tst,  fsm_new_num_tst);

    ASSERT_NEG_A_EQ_B(fsm_float_tst_tr,    fsm_double_tst);
    ASSERT_NEG_A_EQ_B(fsm_double_tst_tr,   fsm_double_tst);
    ASSERT_NEG_A_EQ_B(fsm_new_num_tst_tr,  fsm_new_num_tst);

    ASSERT_NEG_A_EQ_B(drm_double_tst,   drm_double_tst);
    ASSERT_NEG_A_EQ_B(drm_new_num_tst,  drm_new_num_tst);

    using fsm_double_tst_34 = STD_LA::matrix<fs_matrix_engine_tst<double, 3, 4>, test_neg_op_traits_tst>;

    ASSERT_NEG_A_EQ_B(fsm_double_tst_34,  fsm_double_tst_34);
}

void
TestGroup30()
{
    PRINT_FNAME();

    t300();
    t301();
    t302();
    t304();
}

#endif
