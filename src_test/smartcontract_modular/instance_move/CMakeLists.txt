
set(testsrc
	MoveTestSessionHeadDitector.cpp
	MoveTestUtil.cpp
	test_instance_space_move.cpp
	test_make_callserial.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_modular/instance_move)
