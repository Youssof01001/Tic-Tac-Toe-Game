# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TicTacToeApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TicTacToeApp_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\TicTacToeTests_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TicTacToeTests_autogen.dir\\ParseCache.txt"
  "TicTacToeApp_autogen"
  "TicTacToeTests_autogen"
  "googletest\\googlemock\\CMakeFiles\\gmock_autogen.dir\\AutogenUsed.txt"
  "googletest\\googlemock\\CMakeFiles\\gmock_autogen.dir\\ParseCache.txt"
  "googletest\\googlemock\\CMakeFiles\\gmock_main_autogen.dir\\AutogenUsed.txt"
  "googletest\\googlemock\\CMakeFiles\\gmock_main_autogen.dir\\ParseCache.txt"
  "googletest\\googlemock\\gmock_autogen"
  "googletest\\googlemock\\gmock_main_autogen"
  "googletest\\googletest\\CMakeFiles\\gtest_autogen.dir\\AutogenUsed.txt"
  "googletest\\googletest\\CMakeFiles\\gtest_autogen.dir\\ParseCache.txt"
  "googletest\\googletest\\CMakeFiles\\gtest_main_autogen.dir\\AutogenUsed.txt"
  "googletest\\googletest\\CMakeFiles\\gtest_main_autogen.dir\\ParseCache.txt"
  "googletest\\googletest\\gtest_autogen"
  "googletest\\googletest\\gtest_main_autogen"
  )
endif()
