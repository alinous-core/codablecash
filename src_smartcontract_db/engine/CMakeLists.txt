

set(__src
	CdbBinaryObject.cpp
	CdbException.cpp
	CdbLocalCacheManager.cpp
	CdbOid.cpp
	CodableDatabase.cpp
	OidWraper.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db engine)

