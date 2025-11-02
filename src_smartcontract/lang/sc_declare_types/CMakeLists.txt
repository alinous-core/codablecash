

set(__src
	AbstractPrimitiveType.cpp
	AbstractType.cpp
	BoolType.cpp
	ByteType.cpp
	CharType.cpp
	DomType.cpp
	GenericsObjectType.cpp
	IntType.cpp
	ITypeVisitor.cpp
	LongType.cpp
	ObjectType.cpp
	ShortType.cpp
	StringType.cpp
	VoidType.cpp
)

handle_sub(codablecashlib "${__src}" smartcontract lang/sc_declare_types)

