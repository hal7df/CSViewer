project(csviewer)
cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_CXX_FLAGS "-std=gnu++0x ${CMAKE_CXX_FLAGS}")

add_library(CSVReader CSVReader.h CSVReader.cpp)
add_library(Viewer Viewer.h Viewer.cpp)
target_link_libraries(Viewer ncurses)

add_executable(csviewer main.cpp)
target_link_libraries(csviewer CSVReader Viewer)

install(TARGETS csviewer DESTINATION bin)
install(FILES "CSVReader.h" "Viewer.h" DESTINATION obj)
