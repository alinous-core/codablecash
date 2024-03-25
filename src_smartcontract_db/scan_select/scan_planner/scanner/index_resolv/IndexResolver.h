/*
 * IndexResolver.h
 *
 *  Created on: 2020/11/26
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_INDEXRESOLVER_H_
#define SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_INDEXRESOLVER_H_

#include "base/ArrayList.h"

namespace alinous {
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class CodableDatabase;
class AbstractIndexCandidate;
class AbstractColumnsIndexAccess;
class OrIndexCandidate;
class MultipleIndexCandidate;
class SingleColumnIndexAccess;
class OrIndexAccessCollection;
class MultipleColumnIndexAccess;
class AbstractIndexCandidateCollection;

class IndexResolver {
public:
	explicit IndexResolver(CodableDatabase* db);
	virtual ~IndexResolver();

	void analyze(const AbstractIndexCandidate* candidate, VirtualMachine* vm);

	AbstractColumnsIndexAccess* getResult() const noexcept {
		return result;
	}

private:
	AbstractColumnsIndexAccess* doAnalyze(const AbstractIndexCandidate* candidate, VirtualMachine* vm);
	OrIndexAccessCollection* analyzeOr(const OrIndexCandidate* orCandidate, VirtualMachine* vm);
	MultipleColumnIndexAccess* analyzeAnd(const MultipleIndexCandidate* andCandidate, VirtualMachine* vm);

	SingleColumnIndexAccess* handleSingleIndex(const AbstractIndexCandidate* candidate, VirtualMachine* vm);
	AbstractColumnsIndexAccess* handleIndexCollection(const AbstractIndexCandidateCollection* candidate, VirtualMachine* vm);

private:
	CodableDatabase* db;
	AbstractColumnsIndexAccess* result;

};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_INDEXRESOLVER_H_ */
