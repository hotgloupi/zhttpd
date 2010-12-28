
project(md5)
set(md5_srcs md5.c)
find_library(M_LIB m)
add_library(md5 STATIC ${md5_srcs} ${M_LIB})

