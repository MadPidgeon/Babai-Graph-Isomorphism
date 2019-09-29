cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(Catch2 REQUIRED)

if (CMAKE_BUILD_TYPE STREQUAL "coverage" OR CODE_COVERAGE)
    if ("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        message("Building with llvm Code Coverage Tools")

        find_program(LLVM_COV_PATH NAMES "llvm-cov")
        # Warning/Error messages
        if (NOT LLVM_COV_PATH)
            message(FATAL_ERROR "llvm-cov not found! Aborting.")
        endif ()

        # set Flags
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

    elseif (CMAKE_COMPILER_IS_GNUCXX)
        message("Building with lcov Code Coverage Tools")

        # Warning/Error messages
        if (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
            message(WARNING "Code coverage results with an optimized (non-Debug) build may be misleading")
        endif ()
        if (NOT LCOV_PATH)
            message(FATAL_ERROR "lcov not found! Aborting...")
        endif ()
        if (NOT GENHTML_PATH)
            message(FATAL_ERROR "genhtml not found! Aborting...")
        endif ()

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    else ()
        message(FATAL_ERROR "Code coverage requires Clang or GCC. Aborting.")
    endif ()
endif ()

enable_testing()

add_subdirectory(action)
add_subdirectory(coset)
add_subdirectory(datastructures)
add_subdirectory(design_lemma)
add_subdirectory(examples)
add_subdirectory(ext)
add_subdirectory(fhl)
add_subdirectory(group)
add_subdirectory(luks)
add_subdirectory(permutation)
add_subdirectory(unionfind)
add_subdirectory(multi)