cmake_minimum_required(VERSION 3.15)
project(RGS LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(HEADERS

)

set(SOURCES
	"src/Main.cpp"
	"src/Application.cpp")

set(TARGET RGS)
add_executable(${TARGET} ${HEADERS} ${SOURCES})