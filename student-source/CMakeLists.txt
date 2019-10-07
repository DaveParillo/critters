cmake_minimum_required(VERSION 3.1...3.14) 
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# The 'project name' is defined here
project(critters-sandbox VERSION 1.0.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME} 
  ${CMAKE_SOURCE_DIR}/include/add_players.h
  ${CMAKE_SOURCE_DIR}/include/color.h
  ${CMAKE_SOURCE_DIR}/include/critter.h
  ${CMAKE_SOURCE_DIR}/include/direction.h
  add_players.cpp

  # add your source files here

  olympian.cpp olympian.h # delete these files if not using them
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME})


