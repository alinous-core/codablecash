

set(__src
	AccessControlDeclare.cpp
	ArgumentDeclare.cpp
	ArgumentsListDeclare.cpp
	ClassDeclare.cpp
	ClassDeclareBlock.cpp
	ClassExtends.cpp
	ClassImplements.cpp
	ClassName.cpp
	GenericsClassDeclare.cpp
	GenericsGeneratedClassDeclare.cpp
	GenericsParameter.cpp
	ImportDeclare.cpp
	ImportsDeclare.cpp
	MemberVariableDeclare.cpp
	MethodDeclare.cpp
	PackageNameDeclare.cpp
	PackageDeclare.cpp
)

handle_sub(codablecashlib "${__src}" smartcontract lang/sc_declare)

