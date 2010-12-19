
# mod_ssl
file(GLOB mod_ssl_src "*.[ch]pp")
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
