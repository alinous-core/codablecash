/*
 * PowRandomHashManager.cpp
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#include "pow_hash/PowRandomHashManager.h"
#include "pow_hash/PoWHashProgram.h"
#include "pow_hash/PoWForLoop.h"
#include "pow_hash/PoWAesAlgo.h"
#include "pow_hash/PoWSalsaAlgo.h"
#include "pow_hash/PoWSha256Algo.h"
#include "pow_hash/AbstractHashAlgorithmElement.h"
#include "pow_hash/TotalNumberSplitter.h"
#include "pow_hash/PoWAstroBwtAlgo.h"
#include "pow_hash/RandomShaker.h"

#include "astrobwt/AstroBwt.h"

#include "base/StackRelease.h"

#include "osenv/memory.h"

#include "numeric/BigInteger.h"

#include "base_io/ByteBuffer.h"

#include "yescrypt/sha256.h"

using alinous::Mem;

namespace codablecash {

PowRandomHashManager::PowRandomHashManager() {
	this->program = nullptr;
	this->abwt = new AstroBwt();
	this->abwt->setMask(0xffff);
}

PowRandomHashManager::~PowRandomHashManager() {
	delete this->program;
	delete this->abwt;
}

ByteBuffer* PowRandomHashManager::calculate(const BigInteger *hash, const BigInteger *nonce) {
	createProgram(hash, nonce);

	return excecProgram(hash, nonce);
}

ByteBuffer* PowRandomHashManager::excecProgram(const BigInteger *hash, const BigInteger *nonce) {
	char* arrays[4];
	Mem::memset(arrays, 0, 4 * sizeof(char*));

	{
		arrays[0] = new char[32];
		Mem::memset(arrays[0], 0, 32);
		ByteBuffer* bin = hash->toBinary(); __STP(bin);
		Mem::memcpy(arrays[0], bin->array(), bin->limit());
	}
	{
		arrays[1] = new char[32];
		Mem::memset(arrays[1], 0, 32);
		ByteBuffer* bin = nonce->toBinary(); __STP(bin);
		Mem::memcpy(arrays[1], bin->array(), bin->limit());
	}
	{
		arrays[2] = new char[32];
		Mem::memset(arrays[2], 0, 32);
		arrays[3] = new char[32];
		Mem::memset(arrays[3], 0, 32);
	}
	StackArrayRelease<char> __st_0(arrays[0]);
	StackArrayRelease<char> __st_1(arrays[1]);
	StackArrayRelease<char> __st_2(arrays[2]);
	StackArrayRelease<char> __st_3(arrays[3]);


	this->program->execute(arrays);

	{
		SHA256_CTX ctx;
		SHA256_Init(&ctx);

		SHA256_Update(&ctx, arrays[0], 32);
		SHA256_Update(&ctx, arrays[1], 32);
		SHA256_Update(&ctx, arrays[2], 32);
		SHA256_Update(&ctx, arrays[3], 32);

		SHA256_Final((uint8_t *)arrays[3], &ctx);
	}


	return ByteBuffer::wrapWithEndian(arrays[3], 32, true);
}

void PowRandomHashManager::createProgram(const BigInteger *hash, const BigInteger *nonce) {
	delete this->program, this->program = nullptr;

	ArrayList<AbstractHashAlgorithmElement> progList;

	// build for loops
	{
		int numLoops = 5;
		int totalNumOp = 100;
		int totalLoop = numLoops * 20;
		ArrayList<PoWForLoop>* forLoops = creteForLoops(nonce, numLoops, totalNumOp, totalLoop); __STP(forLoops);
		for(int i = 0; i != numLoops; ++i){
			PoWForLoop* loop = forLoops->get(i);

			progList.addElement(loop);
		}
	}

	// build AstroBwt
	{
		int maxLoop = 1;
		for(int i = 0; i != maxLoop; ++i){
			PoWAstroBwtAlgo* bwt = new PoWAstroBwtAlgo(this->abwt);
			progList.addElement(bwt);
		}
	}

	// buid program
	{
		int maxLoop = progList.size();
		RandomShaker shaker(hash, maxLoop);

		this->program = new PoWHashProgram();
		for(int i = 0; i != maxLoop; ++i){
			int pos = shaker.get(i);
			AbstractHashAlgorithmElement* algo = progList.get(pos);
			this->program->addOperand(algo);
		}

		this->program->buildInputs(hash, 0);
	}
}

ArrayList<PoWForLoop>* PowRandomHashManager::creteForLoops(const BigInteger *nonce, int num, int totalNumOp, int totalLoops) const noexcept {
	TotalNumberSplitter splitter(nonce, num, totalNumOp);
	TotalNumberSplitter splitterLoops(nonce, num, totalLoops);

	ArrayList<PoWForLoop>* forLoops = new ArrayList<PoWForLoop>();

	for(int i = 0; i != num; ++i){
		int opeNum = splitter.get(i);
		int loops = splitterLoops.get(i);

		PoWForLoop* l = buildForLoop(nonce, opeNum, loops);
		forLoops->addElement(l);
	}

	return forLoops;
}

PoWForLoop* PowRandomHashManager::buildForLoop(const BigInteger *nonce, int numOp, int loops) const noexcept {
	PoWForLoop* l = new PoWForLoop();
	l->setLoops(loops);

	for(int i = 0; i != numOp; ++i){
		AbstractHashAlgorithmElement* ope = getOperator(nonce, i);

		l->addOperand(ope);
	}

	return l;
}

AbstractHashAlgorithmElement* PowRandomHashManager::getOperator(const BigInteger *nonce, int value) const noexcept {
	int type = value % 3;

	AbstractHashAlgorithmElement* element = nullptr;
	if(type == AbstractHashAlgorithmElement::OP_AES){
		element = new PoWAesAlgo();
	}
	else if(type == AbstractHashAlgorithmElement::OP_SALSA){
		element = new PoWSalsaAlgo();
	}
	else { // if(type == AbstractHashAlgorithmElement::OP_SHA256)
		element = new PoWSha256Algo();
	}

	return element;
}

} /* namespace codablecash */
