cmake_minimum_required(VERSION 3.4)
project(llvm-pass-learn)

# set(ENV{LLVM_HOME} /path/to/installed/llvm/root/dir)
set(ENV{LLVM_HOME} /usr/local/opt/llvm) # brew installed llvm path
# set(ENV{LLVM_DIR} /usr/local/opt/llvm/lib/cmake/llvm) # brew installed llvm path

# we need LLVM_HOME in order to automatically set LLVM_DIR
if(NOT DEFINED ENV{LLVM_HOME})
    message(FATAL_ERROR "$LLVM_HOME is not defined")
else ()
    set(ENV{LLVM_DIR} $ENV{LLVM_HOME}/lib/cmake/llvm)
endif()

find_package(LLVM REQUIRED CONFIG)
# include_directories(/usr/local/opt/llvm/include)

# # AddLLVM needs these
# set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin)
# set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
# include(HandleLLVMOptions) # important: matches compiler flags to LLVM/Clang build
include(AddLLVM)

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
if (${LLVM_VERSION_MAJOR} VERSION_GREATER_EQUAL 10)
    set(CMAKE_CXX_STANDARD 14)
endif ()

add_subdirectory(CustomLegacyPass)  # Use your pass name here.
add_subdirectory(CustomNewPass)  # Use your pass name here.
