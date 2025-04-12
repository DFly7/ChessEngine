cmake_minimum_required(VERSION 3.10)
project(ChessEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCE_FILES
    src/main.cpp
    src/BitBoards.cpp
    src/Move.cpp
    src/GUIStuff/RenderData.cpp
    src/GUIStuff/TextureManager.cpp
    src/GUIStuff/View.cpp
    
    src/UCIInterface.cpp
    src/MoveGenerator.cpp
    src/Evaluation.cpp
    src/Search.cpp
    src/Board.cpp
)

# Create executable first
add_executable(ChessEngine ${SOURCE_FILES})

# Add include directories
target_include_directories(ChessEngine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GUIStuff
)

# Set framework paths
set(SDL2_FRAMEWORK ${CMAKE_CURRENT_SOURCE_DIR}/third_party/SDL2.framework)
set(SDL2_IMAGE_FRAMEWORK ${CMAKE_CURRENT_SOURCE_DIR}/third_party/SDL2_image.framework)

# Add framework include directories
target_include_directories(ChessEngine PRIVATE
    ${SDL2_FRAMEWORK}/Headers
    ${SDL2_IMAGE_FRAMEWORK}/Headers
)

# Link frameworks
target_link_libraries(ChessEngine PRIVATE
    ${SDL2_FRAMEWORK}
    ${SDL2_IMAGE_FRAMEWORK}
    "-framework Cocoa"
    "-framework Metal"
    "-framework AudioUnit"
    "-framework CoreAudio"
    "-framework CoreHaptics"
    "-framework ForceFeedback"
    "-framework GameController"
    "-framework IOKit"
)

# Copy Resources directory to build directory
add_custom_command(TARGET ChessEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources
    ${CMAKE_CURRENT_BINARY_DIR}/Resources
) 