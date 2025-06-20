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
    main.cpp
    MainWindow.cpp
    Board.cpp
    AIPlayer.cpp

)

set(HEADERS

    MainWindow.h
    Board.h
    AIPlayer.h

)

# Create executable with both sources and headers
add_executable(TicTacToe ${SOURCES} ${HEADERS}
)

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
