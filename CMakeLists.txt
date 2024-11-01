cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(ServerClientApp VERSION 0.2)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories (point to the parent directory where Monocypher is located)
include_directories(..)  # Adjust path to Monocypher and other headers

# Add the Monocypher and other sources files
set(MONOCRYPT_SRC
    ../monocypher.c  # Path to the Monocypher source file
    ../shared.c
    ../random.c
    ../config.c
)

# Set output directory for executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/execute`s) 

# Add the server executable
add_executable(server server.c ${MONOCRYPT_SRC})

# Add the client executable
add_executable(client client.c ${MONOCRYPT_SRC})  # Include Monocypher for the client as well

# Conditional library linking based on the platform
if(WIN32)
    # Windows specific libraries
    # Link with Winsock and math library for client/server
    target_link_libraries(server ws2_32 m)  
    target_link_libraries(client ws2_32 m)  
elseif(UNIX)
    # Linux specific libraries
    target_link_libraries(server pthread m bsd)  # Link with pthread, math, and bsd libraries for server
    target_link_libraries(client pthread m bsd)  # Link with pthread, math, and bsd libraries for client
endif()

