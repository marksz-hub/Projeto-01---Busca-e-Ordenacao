cmake_minimum_required(VERSION 3.28)
project(
	trabalho
	VERSION 0.1.0
	LANGUAGES CXX
)

set(
	SRCS

	"src/main.cpp"
)

add_executable(trabalho ${SRCS})
target_include_directories(trabalho PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_features(trabalho PRIVATE cxx_std_17)
target_compile_options(trabalho PRIVATE -Wall -Wextra -pedantic)
