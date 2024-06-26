# 26.06.2024 15:38:11 Genrated by cbp2cmake V2.5.0 Finale X86 
project(hsrevert LANGUAGES C)
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set (CMAKE_INSTALL_PREFIX /usr )
add_executable(hsrevert tools.c hsrevert.c)
install(TARGETS hsrevert RUNTIME DESTINATION bin)
