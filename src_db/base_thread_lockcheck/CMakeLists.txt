set(__src
	ConcurrentGateRecord.cpp
	LockChecker.cpp
	LockingRecord.cpp
	ThreadSpecificLocks.cpp
)

handle_sub(codablecashlib "${__src}" db base_thread_lockcheck)

