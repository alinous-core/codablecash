/*
 * LocalOid2OidLog.h
 *
 *  Created on: 2022/01/13
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_LOCALOID2PUBLISHEDOIDLOG_H_
#define TRX_TRANSACTION_LOCALOID2PUBLISHEDOIDLOG_H_

namespace alinous {
class BtreeOnMemory;
}
using namespace alinous;

namespace codablecash {

class CdbOid;

class LocalOid2PublishedOidLog {
public:
	LocalOid2PublishedOidLog();
	virtual ~LocalOid2PublishedOidLog();

	void addLocalOid2Oid(const CdbOid* lastLocalOid, const CdbOid* oid);
	const CdbOid* getOid(const CdbOid* lastLocalOid);

private:
	BtreeOnMemory* btree;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_LOCALOID2PUBLISHEDOIDLOG_H_ */
