

set(__src
	EscapeTargetCondition.cpp
	EscapeTargetCondittionSetter.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db scan_select/scan_condition/exp_escape)

