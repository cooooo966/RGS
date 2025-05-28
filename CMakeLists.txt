cmake_minimum_required(VERSION 3.15)
project(RGS LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(HEADERS
 "src/Application.h" "src/Window.h" "src/WindowsWindow.h" "src/Base.h" "src/InputCode.h" "src/Maths.h" "src/FrameBuffer.h" "src/Shader/ShaderBase.h" "src/Renderer.h" "src/Shader/BlinnShader.h" "src/Texture.h")

set(SOURCES
	"src/Main.cpp"
 "src/Application.cpp" "src/Window.cpp" "src/WindowsWindow.cpp" "src/FrameBuffer.cpp" "src/Maths.cpp" "src/Shader/BlinnShader.cpp" "src/Renderer.cpp" "src/stb/stb_image.cpp" "src/Texture.cpp")

set(INCLUDE_PATH "src")

set(TARGET RGS)
add_executable(${TARGET} ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PRIVATE ${INCLUDE_PATH})
target_include_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/stb_image)

set(ASSETS_SRC "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DST "$<TARGET_FILE_DIR:${TARGET}>/assets")

set(ASSETS_CMD copy_directory)
set(ASSETS_MSG "Copy Assets")

add_custom_command(
    TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo
    "${ASSETS_MSG}: ${ASSETS_SRC} to ${ASSETS_DST}")

add_custom_command(
    TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E ${ASSETS_CMD}
    "${ASSETS_SRC}" "${ASSETS_DST}")
