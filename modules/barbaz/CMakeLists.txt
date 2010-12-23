
find_package(Boost REQUIRED)
find_package(Sqlite3 REQUIRED)

add_subdirectory(json_spirit)

file(GLOB mod_barbaz_src "*.[ch]pp")
file(GLOB view_src "view/*.[ch]pp")
file(GLOB types_src "types/*.[ch]pp")
file(GLOB db_src "db/*.[ch]pp")
file(GLOB sqlite3_src "sqlite/*.[ch]pp")


list(APPEND mod_barbaz_src
    ${api_hpp}
    ${server_utils_src}
    ${view_src}
    ${types_src}
    ${db_src}
    ${sqlite3_src}
)
add_library(mod_barbaz MODULE ${mod_barbaz_src})
include_directories(json_spirit .)
link_directories(json_spirit)
target_link_libraries(mod_barbaz json_spirit ${SQLITE3_LIBRARIES} ${BITE})
install(TARGETS mod_barbaz LIBRARY DESTINATION ${MODULES_PATH})

