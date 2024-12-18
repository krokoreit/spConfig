
# lib name
set(lib_name spConfig)

#lib's sources (including 'lib_name.cpp' and all other .cpp files)
set(lib_sources spConfig.cpp spConfigBase.cpp spConfigValue.cpp)

# lib's sources' folder ("" for current, "src" for ./src, "src/etc" for .src/etc)
set(lib_sources_folder "src")


# -------------------------------------------------------
# from here on should run automatically
# -------------------------------------------------------
cmake_minimum_required(VERSION 3.6.0)

message("running CMAkeLists.txt for " ${proj_name} "/" ${lib_name} " in " ${CMAKE_CURRENT_SOURCE_DIR})

# library
add_library(${lib_name} INTERFACE)

# source folder
set(src_folder ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT ${lib_sources_folder} EQUAL "")
    string(APPEND src_folder /${lib_sources_folder})
endif()

# target_sources()
foreach(lib_source ${lib_sources})
    target_sources(${lib_name} INTERFACE ${src_folder}/${lib_source})
endforeach(lib_source ${lib_sources})

# target_include_directories
target_include_directories(${lib_name} INTERFACE ${src_folder})

# clean
set(lib_name "")
set(lib_sources "")

