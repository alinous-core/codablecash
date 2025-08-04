/*
 * ManagedUtxoCache.h
 *
 *  Created on: Jul 17, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_DATA_MANAGEMENT_MANAGEDUTXOCACHE_H_
#define BC_WALLET_NET_DATA_MANAGEMENT_MANAGEDUTXOCACHE_H_

#include "base/ArrayList.h"
#include "base/HashMap.h"

using namespace alinous;

namespace codablecash {

class ManagedUtxoCacheRecord;
class UtxoId;
class AbstractUtxo;
class TransactionId;

class ManagedUtxoCache {
public:
	ManagedUtxoCache();
	virtual ~ManagedUtxoCache();

	void reset() noexcept;

	void addUtxo(const AbstractUtxo* utxo, const TransactionId* trxId, int storeType);
	const ManagedUtxoCacheRecord* getManagedUtxoCacheRecord(const UtxoId* utxoId) const noexcept;
	bool hasUtxo(const UtxoId* utxoId) const noexcept;

	void removeUtxo(const UtxoId* utxoId) noexcept;

	void importOtherManagedUtxoCache(const ManagedUtxoCache* other);

	const ArrayList<ManagedUtxoCacheRecord>* getUtxoList() const noexcept {
		return this->list;
	}

private:
	ArrayList<ManagedUtxoCacheRecord>* list;
	HashMap<UtxoId, ManagedUtxoCacheRecord>* map;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_MANAGEMENT_MANAGEDUTXOCACHE_H_ */
