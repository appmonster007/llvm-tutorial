cmake_minimum_required(VERSION 3.4)
project(llvm-pass-learn)

# set(ENV{LLVM_HOME} /path/to/installed/llvm/root/dir)
set(ENV{LLVM_HOME} /usr/local/opt/llvm/include) # brew installed llvm path


# we need LLVM_HOME in order to automatically set LLVM_DIR
if(NOT DEFINED ENV{LLVM_HOME})
    message(FATAL_ERROR "$LLVM_HOME is not defined")
else ()
    set(ENV{LLVM_DIR} $ENV{LLVM_HOME}/lib/cmake/llvm)
endif()

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
if (${LLVM_VERSION_MAJOR} VERSION_GREATER_EQUAL 10)
    set(CMAKE_CXX_STANDARD 14)
endif ()

add_subdirectory(CustomLegacyPass)  # Use your pass name here.
add_subdirectory(CustomNewPass)  # Use your pass name here.
