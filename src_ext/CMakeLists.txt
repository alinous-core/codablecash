
# set(CMAKE_CXX_FLAGS "-std=c++0x -D__cplusplus=201103L -Wall -fmessage-length=0 -O3 -march=native")

# import source codes


add_subdirectory(hash)
add_subdirectory(mod_sqrt)
add_subdirectory(yescrypt)
add_subdirectory(unicode_icu)
add_subdirectory(zip)

message (STATUS "ext source codes ${hashsrc}")
message (STATUS "ext source codes ${mod_sqrt}")
message (STATUS "ext source codes ${yescryptsrc}")
message (STATUS "ext source codes ${unicodesrc}")
message (STATUS "ext source codes ${zip}")


add_library(extlib STATIC extmain.cpp ${hashsrc} ${mod_sqrt} ${yescryptsrc} ${unicodesrc} ${zip})
target_link_libraries(extlib pthread gmp)


