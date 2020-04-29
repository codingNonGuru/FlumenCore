cmake_minimum_required(VERSION 3.0)

project (FlumenCore)

set(CMAKE_BUILD_TYPE None)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fpermissive -g")

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/FlumenCore)

file(GLOB_RECURSE SOURCE_FILES
	"${PROJECT_SOURCE_DIR}/*"
)

include_directories(/home/andrei/Documents/Code/Libraries/glm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
 
add_library(Core SHARED ${SOURCE_FILES})

#target_link_libraries(Core SDL2)