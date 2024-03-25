/*
 * test_node_source.cpp
 *
 *  Created on: 2023/04/08
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_network/NodeIdentifierSource.h"

#include "base_io/ByteBuffer.h"

using namespace codablecash;

TEST_GROUP(TestNodeSourceGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestNodeSourceGroup, case01){
	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);

	int cap = source->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);

	source->toBinary(buff);
	buff->position(0);

	NodeIdentifierSource* source2 = NodeIdentifierSource::createFromBinary(buff); __STP(source2);
	NodeIdentifierSource* source3 = dynamic_cast<NodeIdentifierSource*>(source->copyData()); __STP(source3);

}
