project(design_lemma)

add_library(${PROJECT_NAME} src/${PROJECT_NAME}.cc)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC action coset datastructures)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE)


add_executable(${PROJECT_NAME}Test test/${PROJECT_NAME}Test.cpp)
target_include_directories(${PROJECT_NAME}Test PUBLIC include)
target_link_libraries(${PROJECT_NAME}Test PRIVATE ${PROJECT_NAME} Catch2::Catch2)
add_test(${PROJECT_NAME} ${PROJECT_NAME}Test)