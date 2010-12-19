
cmake_minimum_required(VERSION 2.6)

find_package(PkgConfig)

# mod_itworks
file(GLOB mod_itworks_src "ItWorks/*.[ch]pp")
list(APPEND mod_itworks_src ${api_hpp} ${server_utils_src})
add_library(mod_itworks MODULE ${mod_itworks_src})

if(WIN32)
    target_link_libraries(mod_itworks ws2_32)
    add_custom_command(TARGET mod_itworks
                       POST_BUILD
                       COMMAND copy \"$(TargetPath)\" .)
endif(WIN32)


#####################################################################
# mod_lua
file(GLOB mod_lua_src "Lua/*.[ch]pp")
list(APPEND mod_lua_src ${api_hpp} ${server_utils_src})

if(UNIX)
    find_package(PkgConfig)
    pkg_check_modules(LUA lua5.1)
    pkg_check_modules(LUABIND luabind)
    include_directories(${LUA_INCLUDE_DIRS})
    link_directories(${LUA_LIBRARY_DIRS})
    target_link_libraries(mod_lua ${LUA_LIBRARIES})
else(UNIX)
    #target_link_libraries(mod_lua lua5.1 luabind ws2_32)
endif(UNIX)

if(LUA)
    add_library(mod_lua MODULE ${mod_lua_src})
    target_link_libraries(mod_lua ${LUA_LIBRARIES} ${LUABIND_LIBRARIES})
    install(TARGETS mod_lua LIBRARY DESTINATION ${MODULES_PATH})
else(LUA)
    message("Lua module won't be build")
endif(LUA)

######################################################################
# mod_gzip
file(GLOB mod_gzip_src "Gzip/*.[ch]pp")
list(APPEND mod_gzip_src ${api_hpp} ${server_utils_src})
add_library(mod_gzip MODULE ${mod_gzip_src})
install(TARGETS mod_gzip LIBRARY DESTINATION ${MODULES_PATH})

if(UNIX)
    target_link_libraries(mod_gzip -lz)
endif(UNIX)

if(WIN32)
    target_link_libraries(mod_gzip zlib ws2_32)
    add_custom_command(TARGET mod_gzip
                       POST_BUILD
                       COMMAND copy \"$(TargetPath)\" .)
endif(WIN32)

# mod_cgi
file(GLOB mod_cgi_src "CGI/*.[ch]pp")
list(APPEND mod_cgi_src ${api_hpp} ${server_utils_src})
add_library(mod_cgi MODULE ${mod_cgi_src})
if(WIN32)
    target_link_libraries(mod_cgi ws2_32)
    add_custom_command(TARGET mod_cgi
                       POST_BUILD
                       COMMAND copy \"$(TargetPath)\" .)
endif(WIN32)
install(TARGETS mod_cgi LIBRARY DESTINATION ${MODULES_PATH})

# mod_ssl
file(GLOB mod_ssl_src "SSLNetwork/*.[ch]pp")
list(APPEND mod_ssl_src ${api_hpp} ${server_utils_src})
add_library(mod_ssl MODULE ${mod_ssl_src})
if(UNIX)
    target_link_libraries(mod_ssl ssl)
else(UNIX)
    target_link_libraries(mod_ssl libeay32 ssleay32 ws2_32)
    add_custom_command(TARGET mod_ssl
                       POST_BUILD
                       COMMAND copy \"$(TargetPath)\" .)
endif(UNIX)
install(TARGETS mod_ssl LIBRARY DESTINATION ${MODULES_PATH})


