cmake_minimum_required(VERSION 3.4)
include_directories(${LLVM_INCLUDE_DIRS})

add_llvm_library(
    CustomLegacyPass 
    SHARED
    # List your source files here.
    MyLegacyPass.cpp
    MyLegacyPass.h
)

# # Use C++11 to compile your pass (i.e., supply -std=c++11).
# target_compile_features(CustomLegacyPass PRIVATE cxx_range_for cxx_auto_type)

# LLVM is (typically) built with no C++ RTTI. We need to match that;
# otherwise, we'll get linker errors about missing RTTI data.
# set_target_properties(CustomLegacyPass PROPERTIES
#     COMPILE_FLAGS "-fno-rtti"
# )

if(NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

# Get proper shared-library behavior (where symbols are not necessarily
# resolved when the shared library is linked) on OS X.
if(APPLE)
    set_target_properties(CustomLegacyPass PROPERTIES
        LINK_FLAGS "-undefined dynamic_lookup"
    )
endif(APPLE)
