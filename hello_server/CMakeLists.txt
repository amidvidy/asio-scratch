cmake_minimum_required(VERSION 2.8)
PROJECT(basic_server)

FIND_PACKAGE(Boost 1.5.6 REQUIRED COMPONENTS system thread regex)
If(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

ADD_EXECUTABLE(basic_server basic_server.cpp)
TARGET_LINK_LIBRARIES(basic_server ${USED_LIBS})

ADD_DEFINITIONS(
    -std=c++1y # Or -std=c++0x
    # Other flags
)
