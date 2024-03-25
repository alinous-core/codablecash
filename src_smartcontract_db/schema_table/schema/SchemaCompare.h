/*
 * SchemaCompare.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_SCHEMA_SCHEMACOMPARE_H_
#define SCHEMA_TABLE_SCHEMA_SCHEMACOMPARE_H_

namespace codablecash {

class Schema;

class SchemaCompare {
public:
	int operator() (const Schema* const a, const Schema* const b) const noexcept;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_SCHEMA_SCHEMACOMPARE_H_ */
