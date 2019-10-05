cmake_minimum_required(VERSION 3.1...3.14) 
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(critters-sandbox VERSION 1.0.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME} 
  olympian.cpp olympian.h
  add_players.cpp add_players.h
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME})


