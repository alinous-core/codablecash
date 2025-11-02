/*
 * GenericsTypeHolder.h
 *
 *  Created on: Oct 24, 2025
 *      Author: iizuka
 */

#ifndef ENGINE_SC_ANALYZE_GENERATEDGENERICSTYPEHOLDER_H_
#define ENGINE_SC_ANALYZE_GENERATEDGENERICSTYPEHOLDER_H_

#include "base/ArrayList.h"


namespace alinous {

class GenericsGeneratedClassDeclare;
class GenericsObjectType;
class AnalyzeContext;
class CompilationUnit;

class GeneratedGenericsTypeHolder {
public:
	GeneratedGenericsTypeHolder();
	virtual ~GeneratedGenericsTypeHolder();

	void addAbstractTypeCandidate(GenericsObjectType* candidate);
	void generateGeneratedGenericsClassDeclare(AnalyzeContext* actx);

	void preAnalyzeGenerics(AnalyzeContext* actx);
	void analyzeTypeRefGenerics(AnalyzeContext* actx);
	void analyzeGenerics(AnalyzeContext* actx);

	CompilationUnit* getGenericsGeneratedClassDeclare(const UnicodeString* canonicalName) const noexcept;

private:
	void handleGenericsObjectType(GenericsObjectType* gtype, AnalyzeContext *actx);
	void registerGeneratedClassDeclare(const UnicodeString* canonicalName, GenericsObjectType* gtype, AnalyzeContext *actx);


private:
	ArrayList<CompilationUnit>* list;
	HashMap<UnicodeString, CompilationUnit>* map;

	ArrayList<GenericsObjectType>* inputList;
};

} /* namespace alinous */

#endif /* ENGINE_SC_ANALYZE_GENERATEDGENERICSTYPEHOLDER_H_ */
