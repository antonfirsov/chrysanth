enable_testing()

add_executable(chrys-test Main.cpp GlmTest.cpp VectorTraitsTests.cpp ApproximateComparerTests.cpp)
add_test("chrys-test" chrys-test)
