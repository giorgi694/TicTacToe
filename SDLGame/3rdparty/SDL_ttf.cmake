set (SDL_TTF_VERSION 2.22.0)

include (FetchContent)
FetchContent_Declare (
    SDL_TTF-${SDL_TTF_VERSION}
    URL https://github.com/libsdl-org/SDL_ttf/archive/refs/tags/release-${SDL_TTF_VERSION}.tar.gz
    URL_HASH SHA256=2275d0ddfffa53f0efa628bc1621f662dacbd42467b5a44db99e38255fbb575a
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)

set (sdl2_target_name SDL2::SDL2 CACHE STRING "" FORCE)
set (SDL2TTF_SAMPLES OFF CACHE BOOL "" FORCE)
set (SDL2TTF_HARFBUZZ ON CACHE BOOL "" FORCE)
set (SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)

set (SDL_TTF_SRC_DIR ${FETCHCONTENT_BASE_DIR}/sdl_ttf-${SDL_TTF_VERSION}-src)
set (SDL_TTF_BIN_DIR ${FETCHCONTENT_BASE_DIR}/sdl_ttf-${SDL_TTF_VERSION}-build)

if (NOT SDL_TTF-${SDL_TTF_VERSION}_POPULATED)
    FetchContent_Populate (SDL_TTF-${SDL_TTF_VERSION})

    if (WIN32)
        execute_process (
            COMMAND
                powershell -WindowStyle Hidden -ExecutionPolicy Bypass -File "Get-GitModules.ps1"
            WORKING_DIRECTORY
                ${SDL_TTF_SRC_DIR}/external
        )
    else ()
        execute_process (
            COMMAND
                sh download.sh
            WORKING_DIRECTORY
                ${SDL_TTF_SRC_DIR}/external
        )
    endif ()

    if (NOT BUILD_SHARED_LIBS)
        add_subdirectory (${SDL_TTF_SRC_DIR} ${SDL_TTF_BIN_DIR} EXCLUDE_FROM_ALL)
    else ()
        add_subdirectory (${SDL_TTF_SRC_DIR} ${SDL_TTF_BIN_DIR})
    endif ()
endif ()

add_library (SDL::SDL_ttf ALIAS SDL2_ttf)
