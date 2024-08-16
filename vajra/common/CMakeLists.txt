cmake_minimum_required(VERSION 3.16)
project(vajraServerSdk)

set(CMAKE_CXX_STANDARD 17)

add_library(vajraServerSdk SHARED ConfigurationParser.cpp ConnectionPool.cpp EdrDataTypes.h plugin_base.h hostinfo.cpp hostinfo.h ThreadPool.cpp ThreadPool.h LRUcache.h)
target_link_libraries(vajraServerSdk -lpqxx)
target_link_libraries(vajraServerSdk -lboost_log_setup)
target_link_libraries(vajraServerSdk -lboost_log)
target_link_libraries(vajraServerSdk -lboost_thread)
target_link_libraries(vajraServerSdk -lboost_system)