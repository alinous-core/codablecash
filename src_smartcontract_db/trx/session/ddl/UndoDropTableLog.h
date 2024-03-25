/*
 * UndoDropTableLog.h
 *
 *  Created on: 2023/07/16
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDL_UNDODROPTABLELOG_H_
#define TRX_SESSION_DDL_UNDODROPTABLELOG_H_

#include "trx/session/base/AbstractUndoLog.h"

#include "snapshot/ISnapshotHandler.h"

#include "base/ArrayList.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class CdbTable;
class CdbOid;
class CdbTableColumn;
class TableStartSnapshotCommand;
class SnapshotIndexCommand;
class UndoIndexPart;
class CdbDatabaseSession;
class SnapshotRecordCommand;
class SchemaManager;
class CdbStorageManager;


class UndoDropTableLog : public AbstractUndoLog, public ISnapshotHandler {
public:
	UndoDropTableLog(const UndoDropTableLog& inst);
	UndoDropTableLog();
	virtual ~UndoDropTableLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void putCommand(const AbstractSnapshotCommand* command);

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);

	void setOid(const CdbOid* oid) noexcept;
	void setName(const UnicodeString* name) noexcept;
	void setSchemaName(const UnicodeString* schemaName) noexcept;

	void setDbSession(CdbDatabaseSession* session) noexcept;

private:
	void afterSchemaDetected(const AbstractSnapshotCommand *command);
	void importTalbeColumns(const TableStartSnapshotCommand* cmd);
	void afterTableDetected(const AbstractSnapshotCommand *command);
	void importIndex(const SnapshotIndexCommand* cmd);

	void registerUndoLog() noexcept;

	void importRecordCommand(const SnapshotRecordCommand* recordCommand);

	void recoverIndexes(CdbTransactionManager* trxManager, UndoIndexPart* indexPart, SchemaManager* schemaManager);
	void recoverRecords(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey* key);

private:
	CdbOid* oid;
	UnicodeString* name;
	UnicodeString* schemaName;

	ArrayList<CdbTableColumn>* columns;

	ArrayList<UndoIndexPart>* indexes;

private:
	bool schemaStarted;
	bool tableStarted;
	bool addedlog;

	CdbDatabaseSession* dbsession;

};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDL_UNDODROPTABLELOG_H_ */
