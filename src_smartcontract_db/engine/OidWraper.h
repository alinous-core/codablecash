/*
 * OidWraper.h
 *
 *  Created on: 2023/07/29
 *      Author: iizuka
 */

#ifndef ENGINE_OIDWRAPER_H_
#define ENGINE_OIDWRAPER_H_

namespace codablecash {

class CdbOid;

class OidWraper {
public:
	OidWraper(const OidWraper& inst);
	explicit OidWraper(const CdbOid* oid);
	virtual ~OidWraper();

	int hashCode() const;
	class ValueCompare {
	public:
		int operator() (const OidWraper* const _this, const OidWraper* const object) const noexcept;
	};

private:
	CdbOid* oid;
};

} /* namespace codablecash */

#endif /* ENGINE_OIDWRAPER_H_ */
