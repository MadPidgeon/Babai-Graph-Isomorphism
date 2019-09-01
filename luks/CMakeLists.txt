project(luks)

add_library(${PROJECT_NAME} src/${PROJECT_NAME}.cc)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC group action)
target_include_directories(${PROJECT_NAME} PRIVATE src)

add_executable(${PROJECT_NAME}_test src/${PROJECT_NAME}_test.cc)
target_include_directories(${PROJECT_NAME}_test PUBLIC include)
target_link_libraries(${PROJECT_NAME}_test PUBLIC group action)
target_link_libraries(${PROJECT_NAME}_test PRIVATE catch2 group ${PROJECT_NAME})
