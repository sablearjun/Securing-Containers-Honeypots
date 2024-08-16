cmake_minimum_required(VERSION 3.16)
project(fleetsdk)

set(CMAKE_CXX_STANDARD 17)

add_library(fleetsdk SHARED ConfigurationParser.cpp ConnectionPool.cpp EdrDataTypes.h plugin_base.h hostinfo.cpp hostinfo.h ThreadPool.cpp ThreadPool.h LRUcache.h)
target_link_libraries(fleetsdk -lpqxx)
target_link_libraries(fleetsdk -lboost_log_setup)
target_link_libraries(fleetsdk -lboost_log)
target_link_libraries(fleetsdk -lboost_thread)
target_link_libraries(fleetsdk -lboost_system)