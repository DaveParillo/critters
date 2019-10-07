cmake_minimum_required(VERSION 3.1...3.14) 
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(critters VERSION 1.0.0 LANGUAGES CXX)

find_package(Curses REQUIRED)

set (SOURCES 
  ${CMAKE_SOURCE_DIR}/include/color.h
  ${CMAKE_SOURCE_DIR}/include/direction.h
  ${CMAKE_SOURCE_DIR}/include/critter.h
  critter.cpp
  direction.cpp
  food.h
  game.cpp game.h
  point.cpp point.h
  species.cpp species.h
  stone.h
  view.h
  view_curses.cpp view_curses.h
  main.cpp
)

if(WITH_SOLUTIONS)
  add_definitions(-DWITH_SOLUTIONS)

  add_library(${CMAKE_PROJECT_NAME} 
    ${SOURCES}
    solutions/bear.cpp solutions/bear.h
    solutions/duck.cpp solutions/duck.h
    solutions/lion.cpp solutions/lion.h
    solutions/raccoon.cpp solutions/raccoon.h
    solutions/tiger.cpp solutions/tiger.h
    solutions/wombat.cpp solutions/wombat.h
  )

else()
  add_library(${CMAKE_PROJECT_NAME} 
    ${SOURCES}
  )

endif()

add_executable(${PROJECT_NAME} 
  ${CMAKE_SOURCE_DIR}/include/add_players.h
  add_players.cpp
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME} )

target_link_libraries(${CMAKE_PROJECT_NAME} ${CURSES_LIBRARIES})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CURSES_INCLUDE_DIR}>
)

