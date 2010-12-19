
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

