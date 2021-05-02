message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

add_executable(
    precept_tests
    doctest.cpp
    tests/array.cpp
    tests/half.cpp
    tests/linalg.cpp
    tests/math.cpp
    tests/meta.cpp
    tests/preparable.cpp
    tests/observable.cpp
    tests/ref.cpp
    tests/serializable.cpp
    tests/static_queue.cpp
    tests/static_stack.cpp
    tests/timer.cpp
    )

set_target_properties(
    precept_tests
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    )

target_include_directories(
    precept_tests
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    )

find_package(Threads REQUIRED)
target_link_libraries(precept_tests ${CMAKE_THREAD_LIBS_INIT})

find_package(ZLIB)
if(ZLIB_FOUND)
    message(STATUS "ZLIB_INCLUDE_DIRS: ${ZLIB_INCLUDE_DIRS}")
    message(STATUS "ZLIB_LIBRARIES: ${ZLIB_LIBRARIES}")
    target_link_libraries(precept_tests ZLIB::ZLIB)
endif()

string(RANDOM LENGTH 8 ALPHABET "012345679" SEED)
add_test(
    NAME tests
    COMMAND $<TARGET_FILE:precept_tests> -rs=${SEED}
    )
