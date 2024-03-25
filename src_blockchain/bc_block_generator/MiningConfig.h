/*
 * MiningConfig.h
 *
 *  Created on: 2023/03/29
 *      Author: iizuka
 */

#ifndef BC_BLOCK_GENERATOR_MININGCONFIG_H_
#define BC_BLOCK_GENERATOR_MININGCONFIG_H_

namespace codablecash {

class AddressDescriptor;

class MiningConfig {
public:
	MiningConfig(const MiningConfig& inst);
	MiningConfig();
	virtual ~MiningConfig();

	void setAddressDescriptor(const AddressDescriptor* addressDesc) noexcept;
	const AddressDescriptor* getAddressDescriptor() const noexcept {
		return this->addressDesc;
	}

private:
	AddressDescriptor* addressDesc;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_GENERATOR_MININGCONFIG_H_ */
