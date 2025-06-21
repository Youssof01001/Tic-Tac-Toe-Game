cmake_minimum_required(VERSION 3.16)
project(TicTacToe VERSION 2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable automatic processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Define source and header files separately
set(SOURCES
    src/main.cpp

    src/APP/mainwindow.cpp
    src/Domain/Board.cpp
    src/Domain/AIPlayer.cpp
)

set(HEADERS

    includes/mainwindow.h
    includes/Board.h
    includes/AIPlayer.h
)

# Create executable with both sources and headers
add_executable(TicTacToe ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(TicTacToe PRIVATE Qt6::Core Qt6::Widgets)

# Explicitly wrap headers with MOC (backup method)
qt6_wrap_cpp(MOC_SOURCES ${HEADERS})
target_sources(TicTacToe PRIVATE ${MOC_SOURCES})

# Set properties
set_target_properties(TicTacToe PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# ---------------- Google Test Section ----------------

include(CTest)
enable_testing()

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

# Create test executable
add_executable(TicTacToeTests
    tests/test_ai.cpp
    tests/test_board.cpp
    src/Domain/AIPlayer.cpp
    includes/AIPlayer.h
    src/Domain/Board.cpp
    includes/Board.h
)

# Link GTest
target_link_libraries(TicTacToeTests
    gtest
    gtest_main
)

# Optional: Add Qt includes if needed
target_include_directories(TicTacToeTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Register tests
add_test(NAME TicTacToeTests COMMAND TicTacToeTests)
