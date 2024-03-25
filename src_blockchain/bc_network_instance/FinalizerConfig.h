/*
 * FinalizerConfig.h
 *
 *  Created on: 2023/12/09
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_FINALIZERCONFIG_H_
#define BC_NETWORK_INSTANCE_FINALIZERCONFIG_H_

namespace codablecash {

class NodeIdentifierSource;

class FinalizerConfig {
public:
	FinalizerConfig(const FinalizerConfig& inst);
	FinalizerConfig();
	virtual ~FinalizerConfig();

	void setVoterSource(const NodeIdentifierSource* source) noexcept;
	const NodeIdentifierSource* getVoterSource() const noexcept {
		return this->voterSource;
	}

private:
	NodeIdentifierSource* voterSource;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_FINALIZERCONFIG_H_ */
