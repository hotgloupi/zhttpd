
#####################################################################
# mod_lua
file(GLOB mod_lua_src "*.[ch]pp")
list(APPEND mod_lua_src ${api_hpp} ${server_utils_src})

if(UNIX)
    pkg_check_modules(LUA lua5.1)
    pkg_check_modules(LUABIND luabind)
endif(UNIX)

if(LUA_FOUND)
    if(UNIX)
        include_directories(${LUA_INCLUDE_DIRS})
        link_directories(${LUA_LIBRARY_DIRS})
        add_library(mod_lua MODULE ${mod_lua_src})
        target_link_libraries(mod_lua ${LUA_LIBRARIES} ${LUABIND_LIBRARIES})
    else(UNIX)
        target_link_libraries(mod_lua lua5.1 luabind ws2_32)
    endif(UNIX)
    install(TARGETS mod_lua LIBRARY DESTINATION ${MODULES_PATH})
else(LUA_FOUND)
    message("Lua module won't be build")
endif(LUA_FOUND)
