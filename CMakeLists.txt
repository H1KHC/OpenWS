cmake_minimum_required(VERSION 3.10.2)

file(GLOB OpenWS_Source "src/*.cpp")

include_directories(PUBLIC "include/")
add_library(OpenWS STATIC ${OpenWS_Source})

if (BUILD_SAMPLES)
link_libraries(OpenWS glfw3)

if (WIN32)
elseif (UNIX)
    find_package(GLEW REQUIRED)
    find_package(X11 REQUIRED)
    link_libraries(pthread ${GLEW_LIBRARIES} GLU GL ${X11_LIBRARIES} dl)
endif()

add_executable(2048 src/Sample/2048.cpp)
add_executable(decoration src/Sample/decoration.cpp)
add_executable(Example src/Sample/Example.cpp)
add_executable(Sub-Subwindow src/Sample/Sub-Subwindow.cpp)
add_executable(VariousBuffers src/Sample/VariousBuffers.cpp)
add_executable(windowMessage src/Sample/windowMessage.cpp)
add_executable(Windows src/Sample/Windows.cpp)
endif()