cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(ServerClientApp VERSION 0.5)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories (point to the parent directory where Monocypher is located)
include_directories(src/include)  # Adjust path to Monocypher and other headers

# Add the Monocypher and other sources files
set(LIBS_SRC_SERVER
    src/monocypher.c  # Correct path to the Monocypher source file
    src/addition.c
    src/random.c
    src/config.c
    src/network.c
    src/crypto.c
    src/compress_decompress.c
    src/lzrw3-a.c
    src/txt_reader.c
)
set(LIBS_SRC_CLIENT
    src/monocypher.c  # Correct path to the Monocypher source file
    src/addition.c
    src/random.c
    src/config.c
    src/network.c
    src/crypto.c
    src/client/pin.c
    src/compress_decompress.c
    src/lzrw3-a.c
    src/txt_reader.c
)

# Set output directory for executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

# Add the server executable
add_executable(server server.c ${LIBS_SRC_SERVER})

# Add the client executable
add_executable(client client.c ${LIBS_SRC_CLIENT})  # Include Libs for the client as well

# Conditional library linking based on the platform
if(WIN32)
    # Windows specific libraries
    target_link_libraries(server ws2_32 m)  
    target_link_libraries(client ws2_32 m)  
elseif(UNIX)
    # Linux specific libraries
    target_link_libraries(server m bsd)  # Link with math, and bsd libraries for server
    target_link_libraries(client m bsd)  # Link with math, and bsd libraries for client
endif()
