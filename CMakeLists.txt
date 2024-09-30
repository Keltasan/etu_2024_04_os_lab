cmake_minimum_required(VERSION 3.26)
project(etu_2024_04_os_lab)

add_executable(lab1 lab1_ch1.cpp)

add_executable(lab2 lab2_ch1.cpp)

add_executable(lab3_1 lab3_ch1.cpp)

find_package(OpenMP REQUIRED)

add_executable(lab3_2 lab3_ch2.cpp)

# Проверяем, найдена ли OpenMP, и связываем ее с проектом
if(OpenMP_CXX_FOUND)
    target_link_libraries(lab3_2 PUBLIC OpenMP::OpenMP_CXX)
endif()

# Добавляем флаги компиляции для OpenMP (для MinGW)
if(MINGW)
    target_compile_options(lab3_2 PRIVATE -fopenmp)
    target_link_libraries(lab3_2 PRIVATE -fopenmp)
endif()

add_executable(lab4_1_disp lab4/part_1/dispatch.cpp)

add_executable(lab4_1_read lab4/part_1/reader.cpp)

add_executable(lab4_1_write lab4/part_1/writer.cpp)

add_executable(lab4_2_serv lab4/part_2/server.cpp)

add_executable(lab4_2_client lab4/part_2/client.cpp)