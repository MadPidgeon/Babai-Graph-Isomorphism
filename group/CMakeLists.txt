project(group)

add_library(${PROJECT_NAME} src/${PROJECT_NAME}.cc)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC coset)
target_link_libraries(${PROJECT_NAME} PUBLIC fhl)
target_link_libraries(${PROJECT_NAME} PUBLIC permutation)


target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE permutation)