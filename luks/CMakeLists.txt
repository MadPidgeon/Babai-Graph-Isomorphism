project(luks)

add_library(${PROJECT_NAME} src/${PROJECT_NAME}.cc)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC group action)

target_include_directories(${PROJECT_NAME} PRIVATE src)
