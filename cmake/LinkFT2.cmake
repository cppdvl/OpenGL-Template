include(FetchContent)

macro(LinkFT2 TARGET ACCESS)
    FetchContent_Declare(
            ft2
            GIT_REPOSITORY https://github.com/aseprite/freetype2.git
            GIT_TAG fbbcf50367403a6316a013b51690071198962920
    )

    FetchContent_GetProperties(ft2)

    if (NOT ft2_POPULATED)
        FetchContent_Populate(ft2)


    endif()
    message(STATUS "FT2 Include : ${TARGET} ${ACCESS} ${ft2_SOURCE_DIR}/include" )
    target_include_directories(${TARGET} ${ACCESS} ${ft2_SOURCE_DIR}/include)
    target_link_directories(${TARGET} ${ACCESS} ${ft2_SOURCE_DIR}/include)
endmacro()


