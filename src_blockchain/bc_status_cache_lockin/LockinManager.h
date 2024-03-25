/*
 * LockinManager.h
 *
 *  Created on: 2024/02/11
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_LOCKIN_LOCKINMANAGER_H_
#define BC_STATUS_CACHE_LOCKIN_LOCKINMANAGER_H_

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class LockinManager {
public:
	static const constexpr wchar_t* NAME_LOCKIN_DATA{L"lockin_data"};

	explicit LockinManager(const File* baseDir);
	virtual ~LockinManager();

	void initBlank();

	void open();
	void close() noexcept;

private:
	File* baseDir;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_LOCKIN_LOCKINMANAGER_H_ */
