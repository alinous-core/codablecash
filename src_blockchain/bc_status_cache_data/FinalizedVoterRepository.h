/*
 * FinalizedVoterRepository.h
 *
 *  Created on: 2023/05/08
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_DATA_FINALIZEDVOTERREPOSITORY_H_
#define BC_STATUS_CACHE_DATA_FINALIZEDVOTERREPOSITORY_H_

#include "base/HashMap.h"

namespace alinous {
class File;
class DiskCacheManager;
class Btree;
}
using namespace alinous;

namespace codablecash {

class VoterEntry;
class NodeIdentifier;

class FinalizedVoterRepository {
public:
	static const constexpr wchar_t* NAME_VOTER_STORE{L"votersStore"};

	explicit FinalizedVoterRepository(const File* baseDir);
	virtual ~FinalizedVoterRepository();

	void initBlank();

	void open();
	void close();

	void putVoterEntry(const VoterEntry* entry);

	void exportAllToMap(HashMap<NodeIdentifier, VoterEntry>* votersMap) const noexcept;

private:
	File* baseDir;
	DiskCacheManager* cacheManager;

	Btree* votersStore;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_DATA_FINALIZEDVOTERREPOSITORY_H_ */
