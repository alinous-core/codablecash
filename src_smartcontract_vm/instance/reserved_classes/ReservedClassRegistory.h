/*
 * ReservedClassRegistory.h
 *
 *  Created on: 2020/03/16
 *      Author: iizuka
 */

#ifndef RESERVED_CLASSES_RESERVEDCLASSREGISTORY_H_
#define RESERVED_CLASSES_RESERVEDCLASSREGISTORY_H_

#include "base/ArrayList.h"
#include "base/HashMap.h"

namespace alinous {
class AnalyzedClass;
class CompilationUnit;
class UnicodeString;

class ReservedClassRegistory {
public:
	ReservedClassRegistory();
	virtual ~ReservedClassRegistory();

	const ArrayList<AnalyzedClass>* getReservedClassesList() const noexcept;

	AnalyzedClass* getAnalyzedClass(const UnicodeString* fqn) const noexcept;

	CompilationUnit* makeCompilantUnit(const UnicodeString* packageName) noexcept;

private:
	void addAnalyzedClass(AnalyzedClass* aclass) noexcept;

private:
	ArrayList<AnalyzedClass> list;
	HashMap<UnicodeString, AnalyzedClass> map;
	ArrayList<CompilationUnit>* unitlist;
};

} /* namespace alinous */

#endif /* RESERVED_CLASSES_RESERVEDCLASSREGISTORY_H_ */
