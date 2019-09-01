project(fhl)

add_library(${PROJECT_NAME} src/${PROJECT_NAME}.cc)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(fhl PRIVATE group)
target_link_libraries(fhl PRIVATE permutation)