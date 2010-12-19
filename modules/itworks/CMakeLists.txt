
# mod_itworks
file(GLOB mod_itworks_src "*.[ch]pp")
list(APPEND mod_itworks_src ${api_hpp} ${server_utils_src})
add_library(mod_itworks MODULE ${mod_itworks_src})

if(WIN32)
    target_link_libraries(mod_itworks ws2_32)
    add_custom_command(TARGET mod_itworks
                       POST_BUILD
                       COMMAND copy \"$(TargetPath)\" .)
endif(WIN32)

