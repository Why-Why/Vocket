cmake_minimum_required(VERSION 2.8)

project(Vocket)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(${PROJECT_SOURCE_DIR}/include)

# add_subdirectory(src)
# For src subdirectory
aux_source_directory(${PROJECT_SOURCE_DIR}/src FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Physical P_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Link L_FILES)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_library(Vocket STATIC ${FILES} ${P_FILES} ${L_FILES})

set_target_properties(Vocket PROPERTIES OUTPUT_NAME "Vocket")

# add_subdirectory(test)
# For test subdirectory

aux_source_directory(${PROJECT_SOURCE_DIR}/test FILES)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(test ${FILES})
link_directories(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(test Vocket)
