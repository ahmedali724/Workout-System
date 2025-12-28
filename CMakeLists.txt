cmake_minimum_required(VERSION 3.10)

project(FitnessGo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all .cpp files
file(GLOB_RECURSE SOURCES
    main.cpp
    Model/*.cpp
    UI/*.cpp
    Service/*.cpp
    Utils/*.cpp
    Utils/*.c
)

# Create executable
add_executable(FitnessGo ${SOURCES})

# Tell the compiler where to find header files
target_include_directories(FitnessGo PRIVATE
    Model
    UI
    Service
    Utils
)


# Copy SQLite DB
add_custom_command(TARGET FitnessGo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/fitnessGo.db
        $<TARGET_FILE_DIR:FitnessGo>/fitnessGo.db
)

# Copy the 'frames' folder to the executable directory after build
add_custom_command(TARGET FitnessGo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/AsciFrames
        $<TARGET_FILE_DIR:FitnessGo>/AsciFrames
)

