
find_package(Boost REQUIRED)
find_package(Sqlite3 REQUIRED)

add_subdirectory(md5)
add_subdirectory(json_spirit)

file(GLOB mod_barbaz_src "*.[ch]pp")
file(GLOB view_src "view/*.[ch]pp")
file(GLOB types_src "types/*.[ch]pp")
file(GLOB db_src "db/*.[ch]pp")
file(GLOB sqlite3_src "sqlite/*.[ch]pp")
file(GLOB traversal_src "traversal/*.[ch]pp")
file(GLOB broker_src "broker/*.[ch]pp")


list(APPEND mod_barbaz_src
    ${api_hpp}
    ${server_utils_src}
    ${view_src}
    ${types_src}
    ${db_src}
    ${sqlite3_src}
    ${traversal_src}
    ${broker_src}
)
add_library(mod_barbaz MODULE ${mod_barbaz_src})
include_directories(json_spirit md5 .)
link_directories(json_spirit md5)
target_link_libraries(mod_barbaz json_spirit md5 ${SQLITE3_LIBRARIES})
install(TARGETS mod_barbaz LIBRARY DESTINATION ${MODULES_PATH})

