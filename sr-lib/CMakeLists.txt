# CMakeList.txt : CMake project for sr-lib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(PROJECT_NAME sr-lib)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "include/game/communication.h"
    "include/game/game.h"
    "include/game/memory.h"
    "include/game/offsets.h"
    "include/game/packet.h"
    "include/game/signatures.h"
    "include/game/structs.h"
    "include/inject/detour.h"
    "include/inject/ntinfo.h"
    "include/inject/server.h"
    "include/inject/thread.h"
    "include/util.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "src/dllmain.cpp"
    "src/game/communication.cpp"
    "src/game/game.cpp"
    "src/game/memory.cpp"
    "src/game/offsets.cpp"
    "src/game/packet.cpp"
    "src/game/signatures.cpp"
    "src/hooks.cpp"
    "src/inject/detour.cpp"
    "src/inject/ntinfo.cpp"
    "src/inject/server.cpp"
    "src/inject/thread.cpp"
    "src/util.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "WIN32;"
    "SPEEDRUNNERSAI_EXPORTS;"
    "_WINDOWS;"
    "_USRDLL;"
    "UNICODE;"
    "_UNICODE"
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /std:c++17
        >
        $<$<CONFIG:Release>:
            /Oi;
            /Gy
        >
        /permissive-;
        /sdl;
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG;
        /SUBSYSTEM:WINDOWS
    )
endif()