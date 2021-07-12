cmake_minimum_required(VERSION 3.17)
project(vulkan-learn
        LANGUAGES C CXX)

include(cmake/configure-conan.cmake)
include(cmake/conan.cmake)

if(NOT CONAN_EXPORTED) 
    conan_check()
    conan_add_remote(NAME sunnycase URL https://conan.sunnycase.moe INDEX 0)
endif()

if(CONAN_EXPORTED) # in conan local cache
    message(STATUS "Standard Conan Installation")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup() # NOTE need manmul set cppstd in conanfile.py
else() # in user space
    message(STATUS "Auto Cmake Conan Installation")
    include(${CMAKE_SOURCE_DIR}/cmake/conan.cmake)
    conan_cmake_run(CONANFILE conanfile.py
                    BASIC_SETUP
                    OPTIONS ${CONAN_OPTS}
                    SETTINGS ${CONAN_SETTINGS}
                    BUILD missing)
endif()

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Vulkan REQUIRED)

add_subdirectory(src)
