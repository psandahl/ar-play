project(ar-play)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV 4.4.0 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

set(play-lib
  src/Augmenter.cpp
  src/ChessboardCalib.cpp
  src/ChessboardDetect.cpp
)

add_executable(ar-play src/main.cpp ${play-lib})
target_link_libraries(ar-play PRIVATE ${OpenCV_LIBS})
