cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(PinChanger VERSION 0.2)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories (point to the parent directory where Monocypher is located)
include_directories(../src/include)  # Adjust path to Monocypher and other headers

# Add the Monocypher and other sources files
set(LIBS_SRC
    ../src/monocypher.c  # Correct path to the Monocypher source file
    ../src/client/pin.c
    ../src/config.c
    ../src/txt_reader.c
)

# Set output directory for executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

# Add the re-pinner executable
add_executable(pin_changer pin_changer.c ${LIBS_SRC})
 

