
cmake_minimum_required(VERSION 2.6)

find_package(PkgConfig)

set(itworks OFF)
set(lua OFF)
set(gzip ON)
set(cgi ON)
set(ssl ON)
set(barbaz OFF)
set(modules itworks lua gzip cgi ssl barbaz)

foreach(mod ${modules})
    option(WITH_mod_${mod} "Enable/disable mod_${mod}" ${${mod}})
    message(STATUS "Module mod_${mod} build status: " ${WITH_mod_${mod}})
    if (WITH_mod_${mod})
        add_subdirectory(${mod})
    endif()
    set(WITH_mod_${mod} ${WITH_mod_${mod}} CACHE BOOL "mod_${mod} build")
endforeach(mod)

