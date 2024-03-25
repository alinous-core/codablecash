/*
 * DataHistory.h
 *
 *  Created on: 2023/09/28
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_DATAHISTORY_H_
#define DATA_HISTORY_DATAHISTORY_H_

namespace alinous {
class File;
class SysMutex;
}
using namespace alinous;

namespace codablecash {

class DataHistoryStore;
class TransferedDataId;
class AbstractTransferedData;

class DataHistory {
public:
	explicit DataHistory(const File* baseDir);
	virtual ~DataHistory();

	void createBlankDatabase();
	void open();
	void close();

	void add(const TransferedDataId* dataId, const AbstractTransferedData* data);
	bool hasId(const TransferedDataId* dataId) const;

private:
	SysMutex* mutex;
	DataHistoryStore* store;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_DATAHISTORY_H_ */
