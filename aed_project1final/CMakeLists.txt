cmake_minimum_required(VERSION 3.26)
project(untitled8)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled8
        src/Classes/App.cpp
        src/Classes/Class.cpp
        src/Classes/Lesson.cpp
        src/Classes/Request.cpp
        src/Classes/RequestManager.cpp
        src/Classes/Storage.cpp
        src/Classes/Student.cpp
        src/Classes/StudentsSet.cpp
        src/main/main.cpp)
