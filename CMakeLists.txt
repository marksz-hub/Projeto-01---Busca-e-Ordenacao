cmake_minimum_required(VERSION 3.28)
project(
	trabalho
	VERSION 0.1.0
	LANGUAGES CXX
)

add_executable(trabalho "src/main.cpp")
target_include_directories(trabalho PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(trabalho PRIVATE cxx_std_17)
target_compile_options(trabalho PRIVATE -Wall -Wextra -pedantic)

find_package(benchmark REQUIRED)

add_executable(performance "src/benchmark.cpp")
target_include_directories(performance PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(performance PRIVATE cxx_std_17)
target_compile_options(performance PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(performance PRIVATE pthread benchmark::benchmark)
