

cmake_minimum_required(VERSION 3.0)

project(PotaynoyLazPRJ VERSION 0.2 LANGUAGES C CXX)

# Скомпилировать исполняемый файл из исходников
add_executable(PotaynoyLazServer main.cpp server.cpp game.cpp)

# Скомпоновать исполняемый файл с библиотеками
target_link_libraries(PotaynoyLazServer pthread)

# Установить свойству 'CXX_STANDARD' значение "11",
# а свойству 'CXX_STANDARD_REQUIRED' значение "ON":
set_target_properties(PotaynoyLazServer PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)








