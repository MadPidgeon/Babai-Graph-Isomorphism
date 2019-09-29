project(luks)

add_library(${PROJECT_NAME} src/${PROJECT_NAME}.cc)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC group action)
target_include_directories(${PROJECT_NAME} PRIVATE src)

add_executable(${PROJECT_NAME}-test src/${PROJECT_NAME}-test.cc)
target_include_directories(${PROJECT_NAME}-test PUBLIC include)
target_link_libraries(${PROJECT_NAME}-test PUBLIC group action)
target_link_libraries(${PROJECT_NAME}-test PRIVATE Catch2::Catch2 group ${PROJECT_NAME})
add_test(${PROJECT_NAME} ${PROJECT_NAME}-test)
