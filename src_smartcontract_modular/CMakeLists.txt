
set(__dirsrc "")

# import source codes
add_subdirectory(modular_project)
add_subdirectory(smartcontract_cache)
add_subdirectory(smartcontract_executor)
add_subdirectory(smartcontract_instance)
add_subdirectory(transaction)

message(STATUS "src_smartcontract_modular : ${__dirsrc}")

set_parentv("${__dirsrc}" src_smartcontract_modular top_src_smartcontract_modular)
