
set(__dirsrc "")


add_subdirectory(bc_p2p)
add_subdirectory(bc_p2p_client)
add_subdirectory(bc_p2p_cmd)
add_subdirectory(bc_p2p_cmd_network)
add_subdirectory(bc_p2p_cmd_client)
add_subdirectory(bc_p2p_cmd_client_notify)
add_subdirectory(bc_p2p_cmd_node)
add_subdirectory(bc_p2p_cmd_node_access)
add_subdirectory(bc_p2p_cmd_node_consensus)
add_subdirectory(bc_p2p_info)
add_subdirectory(bc_p2p_processor)
add_subdirectory(bc_p2p_processor_pending)
add_subdirectory(bc_p2p_selector)
add_subdirectory(command_queue)
add_subdirectory(command_queue_cmd)
add_subdirectory(data_history)
add_subdirectory(data_history_data)
add_subdirectory(ipconnect)
add_subdirectory(p2pserver)
add_subdirectory(pubsub)
add_subdirectory(pubsub_cmd)

message(STATUS "src_blockchain : ${__dirsrc}")

set_parentv("${__dirsrc}" src_blockchain_p2p top_src_blockchain_p2p)
