cmake_minimum_required(VERSION 2.8)

file(GLOB My_lib_SRC
	"*.h"	
	"*.cpp"
)

add_library(lib ${My_lib_SRC})