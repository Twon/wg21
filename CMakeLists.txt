cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED PROJECT_NAME)
    set(LA_IS_PRIMARY_PROJECT ON)
endif()

project(wg21_linear_algebra VERSION 0.7.0)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
    conan_basic_setup()
elseif(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()


include(CTest)
include(FetchContent)
include(GNUInstallDirs)
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)

option(LA_BUILD_USING_PCH "Build using precompiled headers" OFF)
option(LA_VERBOSE_TEST_OUTPUT "Write verbose test results" OFF)
option(LA_BUILD_PACKAGE "Build package files" ON)
option(LA_ENABLE_SANITIZERS "Enable Address Sanitizer and Undefined Behaviour Sanitizer if available" OFF)
cmake_dependent_option(LA_INSTALL "Install WG Linear Algebra library" ON "LA_IS_PRIMARY_PROJECT" OFF)
cmake_dependent_option(LA_ENABLE_TESTS "Enable std::math:linear_algebra tests" ON "BUILD_TESTING" OFF)
cmake_dependent_option(LA_BUILD_DEB "Create a DEB" ON "LA_BUILD_PACKAGE" OFF)
cmake_dependent_option(LA_BUILD_RPM "Create a RPM" ON "LA_BUILD_PACKAGE" OFF)

option(MDSPAN_ENABLE_CONCEPTS "" OFF)
FetchContent_Declare(mdspan GIT_REPOSITORY https://github.com/kokkos/mdspan.git GIT_TAG stable)
FetchContent_MakeAvailable(mdspan)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_library(wg21_linear_algebra INTERFACE)

target_include_directories(wg21_linear_algebra
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources(wg21_linear_algebra
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/addition_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/arithmetic_operators.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/debug_helpers.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/division_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/dynamic_vector_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/dynamic_matrix_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/fixed_size_vector_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/fixed_size_matrix_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/forward_declarations.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/matrix.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/matrix_view_engine.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/multiplication_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/negation_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/operation_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/private_support.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/public_support.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/subtraction_traits.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/vector.hpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/linear_algebra/vector_view_engine.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/addition_traits.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/arithmetic_operators.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/debug_helpers.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/division_traits.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/dynamic_vector_engine.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/dynamic_matrix_engine.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/fixed_size_vector_engine.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/fixed_size_matrix_engine.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/forward_declarations.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/matrix.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/matrix_view_engine.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/multiplication_traits.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/negation_traits.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/operation_traits.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/private_support.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/public_support.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/subtraction_traits.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/vector.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/linear_algebra/vector_view_engine.hpp>
)

target_compile_features(wg21_linear_algebra
    INTERFACE
        cxx_std_20
)

#target_compile_definitions(wg21_linear_algebra
#    INTERFACE
#        LA_USE_MDSPAN
#)

target_link_libraries(wg21_linear_algebra
    INTERFACE
        std::mdspan
)

add_library(wg21_linear_algebra::wg21_linear_algebra ALIAS wg21_linear_algebra)

# message("VAR la_enable_tests is: " ${LA_ENABLE_TESTS})

if (${LA_ENABLE_TESTS})
    add_subdirectory(tests)
endif()

if (${LA_INSTALL})
    # Hierarchically copy headers to the install dir
    install (
        DIRECTORY
            "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN
            "*.hpp"
    )

    install(
        TARGETS wg21_linear_algebra
        DESTINATION lib/cmake/wg21_linear_algebra
        EXPORT wg21_linear_algebra-target
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(
        EXPORT wg21_linear_algebra-target
        NAMESPACE wg21_linear_algebra::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wg21_linear_algebra"
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/wg21_linear_algebra-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/wg21_linear_algebra-config.cmake
        INSTALL_DESTINATION
            "${CMAKE_INSTALL_LIBDIR}/cmake/wg21_linear_algebra"
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/wg21_linear_algebra-version.cmake
        VERSION ${PROPAGATE_CONST_VERSION}
        COMPATIBILITY SameMajorVersion
        ARCH_INDEPENDENT
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/wg21_linear_algebra-config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/wg21_linear_algebra-version.cmake
        DESTINATION
            "${CMAKE_INSTALL_LIBDIR}/cmake/wg21_linear_algebra"
    )


    if (NOT LA_BUILD_PACKAGE)
        return()
    endif()

    list(APPEND source-generators TBZ2 TGZ TXZ ZIP)

    if (CMAKE_HOST_WIN32)
        list(APPEND binary-generators "WIX")
    endif()

    if (LA_PACKAGE_DEB AND CMAKE_HOST_LINUX)
        list(APPEND binary-generators "DEB")
    endif()

    if (LA_BUILD_RPM AND CMAKE_HOST_LINUX)
        list(APPEND binary-generators "RPM")
    endif()


    set(CPACK_SOURCE_GENERATOR ${source-generators})
    set(CPACK_GENERATOR ${binary-generators})

    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")

    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Bob Steagall")

    list(APPEND CPACK_SOURCE_IGNORE_FILES /.git/ /build*/ .gitignore .DS_Store)

    include(CPack)
endif()
