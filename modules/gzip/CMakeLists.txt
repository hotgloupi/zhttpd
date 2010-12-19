
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
