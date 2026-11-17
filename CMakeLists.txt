cmake_minimum_required(VERSION 4.4)
project(enet_test_project_01)

set(CMAKE_CXX_STANDARD 20)

add_executable(enet_test_project_01
        main.cpp
        src/MyServer.cpp
        src/MyClient.cpp
        src/MyClient.h
        src/Utils.cpp
        src/Utils.h
)

target_include_directories(enet_test_project_01 PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(enet_test_project_01 PUBLIC ${PROJECT_SOURCE_DIR}/libs/enet-1.3.18/include)

target_link_libraries(enet_test_project_01 PUBLIC ${PROJECT_SOURCE_DIR}/libs/enet-1.3.18/enet64.lib)

target_link_libraries(enet_test_project_01 PUBLIC ws2_32)
target_link_libraries(enet_test_project_01 PUBLIC winmm)