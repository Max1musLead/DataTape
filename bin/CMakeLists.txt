add_executable(DataProgram main.cpp)

target_link_libraries(DataProgram PRIVATE TAPE PARSE)

target_include_directories(DataProgram PUBLIC ${PROJECT_SOURCE_DIR})