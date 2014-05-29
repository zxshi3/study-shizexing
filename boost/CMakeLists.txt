project (boost_study)

set(BOOST_ROOT "/Users/zexings/code/boost/modular-boost")
message("Boost root : "${BOOST_ROOT})
include_directories(${BOOST_ROOT})
include_directories(${PROJECT_SOURCE_DIR})
link_directories(${BOOST_ROOT}/stage/lib)
#set(CMAKE_C_FLAGS "-std=c++11")
set(CMAKE_C_FLAGS "-Wc++11-extensions")

add_executable( boost_study
    thread.cpp
)

target_link_libraries(boost_study 
    boost_thread 
    boost_system
)

