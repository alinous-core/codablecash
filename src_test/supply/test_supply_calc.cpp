/*
 * test_supply_calc.cpp
 *
 *  Created on: 2022/04/10
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"
#include <cstdio>


TEST_GROUP(TestSupplyCalcGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

static uint64_t atom = 10000 * 10000;

static uint64_t secPerDay = 86400;
static uint64_t secPerBlock = 5 * 60;
static uint64_t firstReward = 64;

static bool rewordChange(uint64_t height){
	uint64_t blocksPerDay = secPerDay / secPerBlock;
	uint64_t mod = height % (6144 / 2); // (blocksPerDay * 30);

	return mod == 0;
}

static uint64_t blockReword(uint64_t lastReward) {
	return lastReward * 100 / 101
			;
}

TEST(TestSupplyCalcGroup, case01){

	//FileOutputStream*

	uint64_t total = 0;

	uint64_t days = 365 * 200;

	uint64_t reward = firstReward * atom;

	uint64_t blockPerDay = secPerDay / secPerBlock;

	uint64_t maxLoop = blockPerDay * days;
	uint64_t height = 0;
	for(int i = 1; i != maxLoop; ++i){
		height = i;

		if(rewordChange(height)){
			reward = blockReword(reward);
		}

		if(reward == 0){
			break;
		}

		total += reward;
	}

	uint64_t noSupplyDays = height / blockPerDay;
/*
	printf("total : %lu\n", total / atom);
	printf("last reward atom %lu\n", reward);
	printf("last height %lu\n", height);
	printf("noSupplyDays %lu, year %lu\n", noSupplyDays, noSupplyDays / 365);
	*/
}

