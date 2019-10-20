project(datastructures)

add_library(${PROJECT_NAME} src/${PROJECT_NAME}.cc src/RelationalStructureTextRepresentation.cpp src/RelationStructureToVectors.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC ext unionfind)

target_include_directories(${PROJECT_NAME} PRIVATE src)

add_executable(${PROJECT_NAME}Test test/${PROJECT_NAME}Test.cpp)
target_include_directories(${PROJECT_NAME}Test PUBLIC include)
target_link_libraries(${PROJECT_NAME}Test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
add_test(${PROJECT_NAME} ${PROJECT_NAME}Test)