
set(__dirsrc "")


# import source codes

add_subdirectory(aes256cbc)
add_subdirectory(astrobwt)
add_subdirectory(crypto)
add_subdirectory(ecda)
add_subdirectory(musig)
add_subdirectory(ripemd160)
add_subdirectory(zip)

set_parentv("${__dirsrc}" src top_src)
