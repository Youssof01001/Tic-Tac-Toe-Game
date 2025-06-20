cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(TicTacToe VERSION 0.1 LANGUAGES CXX)

# ---------------- Global C++ Settings ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------- Qt Configuration ----------------
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Enable automatic Qt features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---------------- Application Source Files ----------------
set(APP_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Board.cpp
    Board.h
    AIPlayer.cpp
    AIPlayer.h
    LoginDialog.cpp
    LoginDialog.h
    resources.qrc
)

# ---------------- Main Application Target ----------------
qt_add_executable(TicTacToeApp
    MANUAL_FINALIZATION
    ${APP_SOURCES}
)

target_link_libraries(TicTacToeApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(TicTacToeApp PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TicTacToe
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

install(TARGETS TicTacToeApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(TicTacToeApp)

# ---------------- GoogleTest Section (Manual Clone) ----------------

enable_testing()

# Add manually cloned googletest folder
# It must contain CMakeLists.txt inside
add_subdirectory(googletest)

# ---------------- Test Sources ----------------
add_executable(TicTacToeTests
    test_board.cpp
    test_ai.cpp
    Board.cpp
    Board.h
    AIPlayer.cpp
    AIPlayer.h
)

target_include_directories(TicTacToeTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(TicTacToeTests
    gtest
    gtest_main
)

add_test(NAME TicTacToeTests COMMAND TicTacToeTests)

# Optional: make tests depend on the main app (not required)
# add_dependencies(TicTacToeTests TicTacToeApp)
