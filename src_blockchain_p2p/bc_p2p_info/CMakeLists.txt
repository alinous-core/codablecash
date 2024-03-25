

set(__src
	NodeIdentifierDataFactory.cpp
	P2pDnsManager.cpp
	P2pNodeDatabase.cpp
	P2pNodeRecord.cpp
	P2pNodeRecordDataFactory.cpp
	P2pNodeRecordReference.cpp
	P2PZoneConnectionInfo.cpp
	ZoneNodeKey.cpp
	ZoneNodeKeyFactory.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_info)

