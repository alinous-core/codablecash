/*
 * ImportsDeclare.h
 *
 *  Created on: 2019/06/17
 *      Author: iizuka
 */

#ifndef SC_DECLARE_IMPORTSDECLARE_H_
#define SC_DECLARE_IMPORTSDECLARE_H_

#include "engine/sc/CodeElement.h"
#include "base/ArrayList.h"

namespace alinous {
class ImportDeclare;
class AnalyzeContext;
class AbstractType;

class ImportsDeclare : public CodeElement {
public:
	ImportsDeclare();
	virtual ~ImportsDeclare();

	void addImport(ImportDeclare* imp) noexcept;
	//bool hasImportClass(const UnicodeString* fqn);

	void preAnalyze(AnalyzeContext* actx);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	const ArrayList<ImportDeclare>* getImports() const noexcept;

	ImportsDeclare* generateGenericsImplement(HashMap<UnicodeString, AbstractType> *input);
	void replaceImport(const UnicodeString* from, const UnicodeString* to);

private:
	ArrayList<ImportDeclare> list;
};

} /* namespace alinous */

#endif /* SC_DECLARE_IMPORTSDECLARE_H_ */
