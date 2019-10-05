cmake_minimum_required(VERSION 3.1...3.14) 
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(critters VERSION 1.0.0 LANGUAGES CXX)

find_package(Curses REQUIRED)

add_library(${CMAKE_PROJECT_NAME} 
  color.cpp color.h
  critter.cpp critter.h
  direction.cpp direction.h
  food.h olympian.h
  game.cpp game.h
  point.cpp point.h
  species.cpp species.h
  stone.h
  view.h
  view_curses.cpp view_curses.h
  main.cpp
  solutions/bear.cpp solutions/bear.h
  solutions/duck.cpp solutions/duck.h
  solutions/lion.cpp solutions/lion.h
  solutions/raccoon.cpp solutions/raccoon.h
  solutions/tiger.cpp solutions/tiger.h
  solutions/wombat.cpp solutions/wombat.h
)

add_executable(${PROJECT_NAME} 
  add_players.cpp add_players.h
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME} )

target_link_libraries(${CMAKE_PROJECT_NAME} ${CURSES_LIBRARIES})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CURSES_INCLUDE_DIR}>
)

