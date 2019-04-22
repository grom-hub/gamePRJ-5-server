

cmake_minimum_required(VERSION 3.0)


# Задать характеристики проекта "MyProject":
project(PotaynoyLazPRJ VERSION 0.2 LANGUAGES C CXX)


# Скомпилировать исполняемый файл "MyExecutable" из
# исходников "ObjectHandler.c", "TimeManager.c" и "MessageGenerator.c":
add_executable(PotaynoyLazServer main.cpp server.cpp game.cpp)


# Скомпоновать исполняемый файл "MyExecutable" с
# библиотеками "JsonParser", "SocketFactory" и "BrowserInvoker":
target_link_libraries(PotaynoyLazServer pthread)


# Установить свойству 'CXX_STANDARD' значение "11",
# а свойству 'CXX_STANDARD_REQUIRED' значение "ON":
set_target_properties(PotaynoyLazServer PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)








