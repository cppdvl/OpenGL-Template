include(FetchContent)

macro(LinkOpenFileDialog TARGET ACCESS)
    FetchContent_Declare(
            ofd
            GIT_REPOSITORY https://github.com/dfranx/ImFileDialog
            GIT_TAG 50b33037bb9ceedff6575f31ab7b748abaae054b
    )

    FetchContent_GetProperties(ofd)

    if (NOT ofd_POPULATED)
        FetchContent_Populate(ofd)
    endif()
    message(STATUS "OFD Include : ${TARGET} ${ACCESS} ${ofd_SOURCE_DIR}" )
    target_include_directories(${TARGET} ${ACCESS} ${ofd_SOURCE_DIR})
    target_link_libraries(${TARGET} ${ACCESS} ofd)
    add_dependencies(${TARGET} ofd)
endmacro()