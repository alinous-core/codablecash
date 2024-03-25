/*
 * RecordValueConverter.h
 *
 *  Created on: 2020/09/21
 *      Author: iizuka
 */

#ifndef TABLE_STORE_RECORDVALUECONVERTER_H_
#define TABLE_STORE_RECORDVALUECONVERTER_H_
#include <cstdint>

namespace codablecash {

class CdbTableColumn;
class AbstractCdbValue;
class CdbRecord;
class CdbDatabaseSession;
class CdbOid;

class RecordValueConverter {
public:
	RecordValueConverter(CdbTableColumn* column, const AbstractCdbValue* defaultValue);
	RecordValueConverter(int pos, bool isnotnull, uint8_t cdbType, int length, const AbstractCdbValue* defaultValue);
	virtual ~RecordValueConverter();

	CdbRecord* processUpdate(const CdbRecord* record);

	void setDatabaseSession(CdbDatabaseSession* session) noexcept;

private:
	AbstractCdbValue* getModifiedValue(const AbstractCdbValue* lastValue);
	AbstractCdbValue* handleNullValue();
	AbstractCdbValue* handleStringType(const AbstractCdbValue* lastValue);

	void addUndoRecordValueLog(const CdbOid* recordOid, const AbstractCdbValue* lastValue, const AbstractCdbValue* newValue);

private:
	//CdbTableColumn* column;
	int pos;
	bool isnotnull;
	uint8_t cdbType;
	int length;

	const AbstractCdbValue* defaultValue;

	CdbDatabaseSession* session;
};

} /* namespace codablecash */

#endif /* TABLE_STORE_RECORDVALUECONVERTER_H_ */
