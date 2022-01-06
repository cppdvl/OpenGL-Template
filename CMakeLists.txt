cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(OpenGLTemplate CXX)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE  True)
set(CMAKE_CXX_STANDARD 17)
#===========================================================================================
# OPTIONAL LIBRARiES
#
# The following libraries can be pulled down and added to the project by turnning the
# desired options on. Open an issue for additional default support.
# See: https://github.com/Hoshiningen/OpenGL-Template/issues

set(ENABLE_ASSIMP   OFF CACHE BOOL "Add Open Asset Import Library (assimp) to the project" FORCE)

#===========================================================================================
# GLAD CONFIGURATION
#
# Configure GLAD loader generation variables (lines below pulled from the GLAD top-level
# CMakeLists.txt file, with slight modifications.
# Refer to https://github.com/Dav1dde/glad for more information)

set(GLAD_OUT_DIR        "${CMAKE_CURRENT_BINARY_DIR}" CACHE STRING "Output directory" FORCE)
set(GLAD_PROFILE        "core" CACHE STRING "OpenGL profile" FORCE)
set(GLAD_API            "gl=3.3" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest" FORCE)
set(GLAD_GENERATOR      "c" CACHE STRING "Language to generate the binding for" FORCE)
set(GLAD_EXTENSIONS     "" CACHE STRING "Path to extensions file or comma separated list of extensions, if missing all extensions are included" FORCE)
set(GLAD_SPEC           "gl" CACHE STRING "Name of the spec" FORCE)
set(GLAD_ALL_EXTENSIONS OFF CACHE BOOL "Include all extensions instead of those specified by GLAD_EXTENSIONS" FORCE)
set(GLAD_NO_LOADER      OFF CACHE BOOL "No loader" FORCE)
set(GLAD_REPRODUCIBLE   OFF CACHE BOOL "Reproducible build" FORCE)

#===========================================================================================

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set directory paths
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(GUI_DIR ${SOURCE_DIR}/gui)
set(IMGUI_DIR ${SOURCE_DIR}/gui/imgui)
set(IMGUI_BACKEND_DIR ${IMGUI_DIR}/glfw_opengl3)
set(OBJ_LDR ${SOURCE_DIR}/objloader)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(sources
        source/objloader/objloader.cpp
        source/clioptions/clioptions.h
        source/fontrenderer/fntrenderer.h
        source/fontrenderer/fntrender.cpp
        source/shader/shader.h
        source/backnrenderer/backnrenderer.h
        source/backnrenderer/back/backend.cpp
        source/backnrenderer/renderer/renderer.cpp)

set(imguisources
    ${GUI_DIR}/openfile/ImFileDialog.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp)


add_executable(opengl-template ${SOURCE_DIR}/main.cpp ${sources} ${imguisources} ${includes})
add_executable(guidemo ${IMGUI_DIR}/imgui_demo.cpp ${imguisources} ${includes})
add_executable(objloader ${OBJ_LDR}/objloader.cpp ${OBJ_LDR}/objloadertest.cpp source/objloader/objloader.h ${imguisources})
add_executable(fnttest ${SOURCE_DIR}/fontrenderer/test.cpp ${sources}  )

# Perform dependency linkage
include(${CMAKE_DIR}/LinkGLFW.cmake)
include(${CMAKE_DIR}/LinkGLAD.cmake)
include(${CMAKE_DIR}/LinkGLM.cmake)
include(${CMAKE_DIR}/LinkOpenFileDialog.cmake)
include(${CMAKE_DIR}/LinkSTB.cmake)
include(${CMAKE_DIR}/LinkFT2.cmake)


LinkGLFW(opengl-template PRIVATE)
LinkGLFW(guidemo PRIVATE)
LinkGLFW(objloader PRIVATE)
LinkGLFW(fnttest PRIVATE)

LinkGLAD(opengl-template PRIVATE)
LinkGLAD(guidemo PRIVATE)
LinkGLAD(objloader PRIVATE)
LinkGLAD(fnttest PRIVATE)


LinkGLM(opengl-template PRIVATE)
LinkGLM(objloader PRIVATE)
LinkGLM(fnttest PRIVATE)

LinkSTB(opengl-template PRIVATE)
LinkSTB(objloader PRIVATE)


find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    target_include_directories(opengl-template PRIVATE 
        ${OPENGL_INCLDUE_DIRS}
        ${IMGUI_DIR}
        ${IMGUI_BACKEND_DIR})
    target_link_libraries(opengl-template PRIVATE ${OPENGL_LIBRARIES})
    target_include_directories(guidemo PRIVATE 
        ${OPENGL_INCLDUE_DIRS}
        ${IMGUI_DIR}
        ${IMGUI_BACKEND_DIR})
    target_link_libraries(guidemo PRIVATE ${OPENGL_LIBRARIES})

    message(STATUS "IMGUI Dir: ${IMGUI_DIR}")
    message(STATUS "IMGUI Backend Dir: ${IMGUI_BACKEND_DIR}")
    target_include_directories(objloader PRIVATE
        ${OPENGL_INCLDUE_DIRS}
        ${IMGUI_DIR}
        ${IMGUI_BACKEND_DIR}
        ${GUI_DIR}/openfile)
    target_include_directories(fnttest PRIVATE
            ${OPENGL_INCLDUE_DIRS}
            ${SOURCE_DIR}/clioptions
            ${SOURCE_DIR}/shader
            ${SOURCE_DIR}/backnrenderer
            ${SOURCE_DIR}/fontrenderer
            ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ft2/include)
    target_link_libraries(fnttest PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ft2/include/ft2.lib
            ${OPENGL_LIBRARIES}
            )
else()
    message(FATAL_ERROR "OpenGL was not found on the system")
endif()

# Handle the optional libraries
if (${ENABLE_ASSIMP})
    include(${CMAKE_DIR}/LinkASSIMP.cmake)
    LinkASSIMP(opengl-template PRIVATE)
endif()

# Enable C++17
set_target_properties(opengl-template PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
set_target_properties(guidemo PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

# Set project folders
set_target_properties(opengl-template PROPERTIES FOLDER ${PROJECT_NAME})
#set_target_properties(guidemo PROPERTIES FOLDER ${PROJECT_NAME})



#CUSTOM COMMMANDS
add_custom_command(
        TARGET fnttest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/text.vs.glsl  $<TARGET_FILE_DIR:fnttest>
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/text.fs.glsl  $<TARGET_FILE_DIR:fnttest>
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts/consolas.ttf  $<TARGET_FILE_DIR:fnttest>

)



