

set(__src
	AbstractLikeMatch.cpp
	BeginWithMatch.cpp
	CompleteMatch.cpp
	EndWithMatch.cpp
	LikeExpressionProcessor.cpp
	LikeTokenListCursor.cpp
	MatchAll.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db scan_select/scan_condition/exp_like)
