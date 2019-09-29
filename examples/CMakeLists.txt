add_executable(babai_algorithm src/babai_algorithm.cc)
target_link_libraries(babai_algorithm PRIVATE luks permutation)

add_executable(configurations src/configurations.cc)
target_link_libraries(configurations PRIVATE datastructures design_lemma)

add_executable(cosets_and_pullbacks src/cosets_and_pullbacks.cc)
target_link_libraries(cosets_and_pullbacks PRIVATE permutation group action)

add_executable(groups_and_permutations src/groups_and_permutations.cc)
target_link_libraries(groups_and_permutations PRIVATE permutation group)

add_executable(luks_algorithm src/luks_algorithm.cc)
target_link_libraries(luks_algorithm PRIVATE luks)

add_executable(luks_algorithm_test test/luks_algorithm_test.cc)
target_link_libraries(luks_algorithm_test PRIVATE luks Catch2::Catch2)


