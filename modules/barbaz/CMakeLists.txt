
#add_subdirectory(json_spirit)

file(GLOB mod_barbaz_src "*.[ch]pp")
list(APPEND mod_barbaz_src ${api_hpp} ${server_utils_src})
add_library(mod_barbaz MODULE ${mod_barbaz_src})
install(TARGETS mod_barbaz LIBRARY DESTINATION ${MODULES_PATH})

