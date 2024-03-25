/*
 * LocalOid2OidLog.cpp
 *
 *  Created on: 2022/01/13
 *      Author: iizuka
 */

#include "trx/transaction/LocalOid2PublishedOidLog.h"

#include "btree_memory/BtreeOnMemory.h"

#include "btree/BtreeConfig.h"

#include "btreekey/BtreeKeyFactory.h"

#include "base/StackRelease.h"

#include "engine/CdbOid.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"
#include "schema_table/record/table_record/CdbKeyFactory.h"

namespace codablecash {

LocalOid2PublishedOidLog::LocalOid2PublishedOidLog() {
	BtreeConfig* config = new BtreeConfig();
	config->nodeNumber = 4;
	BtreeKeyFactory* factory = new CdbKeyFactory(); __STP(factory);

	this->btree = new BtreeOnMemory(config, factory);
}

LocalOid2PublishedOidLog::~LocalOid2PublishedOidLog() {
	delete this->btree;
}

void LocalOid2PublishedOidLog::addLocalOid2Oid(const CdbOid* lastLocalOid,	const CdbOid* oid) {
	AbstractCdbKey* key = lastLocalOid->toKey(); __STP(key);

	this->btree->putData(key, oid);
}

const CdbOid* LocalOid2PublishedOidLog::getOid(const CdbOid* lastLocalOid) {
	AbstractCdbKey* key = lastLocalOid->toKey(); __STP(key);

	const IBlockObject* obj = this->btree->findByKey(key);
	const CdbOid* ret = dynamic_cast<const CdbOid*>(obj);

	assert(ret != nullptr);

	return ret;
}

} /* namespace codablecash */
