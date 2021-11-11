cmake_minimum_required(VERSION 3.20)
project(untitled)

find_package(fmt REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_compile_options("-Ofast" "-g" "-Wall" "-Wextra" "-Wpedantic" "-Wconversion" "-fsanitize=address,leak,undefined" "-funroll-loops")

set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address,leak,undefined -fno-omit-frame-pointer -flto")

add_executable(untitled
        main.cpp
        ./src/Math/Vector.cpp
        src/Scene/Object/Sphere.cpp
        ./src/Scene/Scene.cpp
        ./src/Scene/Camera.cpp
        ./src/Util/Random.cpp
        src/Scene/AABoundingBox.cpp
        src/Scene/Object/BVHNode.cpp
        )

target_link_libraries(untitled
        fmt::fmt
        )


