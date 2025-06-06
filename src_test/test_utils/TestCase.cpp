/*
 * TestCase.cpp
 *
 *  Created on: 2018/05/06
 *      Author: iizuka
 */

#include "test_utils/TestCase.h"
#include "test_utils/TestGroup.h"
#include "test_utils/TestGroupActions.h"
#include "test_utils/Check.h"
#include "test_utils/TestEnv.h"
#include "test_utils/TestParams.h"
#include "base/UnicodeString.h"

#include <chrono>

#include "base_io_stream/Writer.h"

#include "base/Exception.h"

#include "base/StackRelease.h"
using namespace std::chrono;

namespace alinous {

TestCase::TestCase(TestGroup* group, const wchar_t* name, TestGroupActions* setup, const char* file, int line) noexcept {
	this->group = group;
	this->name = new UnicodeString(name);
	this->setup = setup;
	this->file = new UnicodeString(file);
	this->line = line;
	this->checks = new ArrayList<Check>();
	this->env = new TestEnv(group, this);

	this->done = false;
	this->failed = false;
	this->microsec = 0;

	this->setup->setNames(group->getName(), this->name);
	this->setup->setTestEnv(this->env);

	group->addTestCase(this->name, this);
}

TestCase::~TestCase() noexcept {
	delete this->name;
	delete this->setup;
	delete this->file;

	this->checks->deleteElements();
	delete this->checks;
}

void TestCase::doTest(TestParams* params) {
	const char* result = "OK";

	setDone();
	try{
		this->setup->setup();
	}catch(...){
		setFailed();
		if(params->isV()){
			printf("  %ls [%ls at %d]... failed in setup()\n", this->name->towString(), this->file->towString(), getLine());
		}
		return;
	}

	uint64_t start, end;
	try{
		Check::getThreadKeyRegistory()->registerTestCase(this);

		start = Os::getMicroSec();
		testBody();
		end = Os::getMicroSec();
	}
	catch(Exception* e){
		__STP(e);
		end = Os::getMicroSec();
		setFailed();
		if(params->isV()){
			printf("  !!!!! %ls [%ls at %d]... failed. Exception was thrown on test body.\n", this->name->towString(), this->file->towString(), getLine());
		}

		UnicodeString* exstr = e->getFileInfo(); __STP(exstr);
		const char* excstr = exstr->toCString();
		StackArrayRelease<const char> __st_excstr(excstr);

		printf("%s\n", excstr);
	}
	catch(...){
		end = Os::getMicroSec();
		setFailed();
		if(params->isV()){
			printf("  !!!!! %ls [%ls at %d]... failed. Exception was thrown on test body.\n", this->name->towString(), this->file->towString(), getLine());
		}
	}
	this->microsec = end - start;

	try{
		this->setup->teardown();
	}
	catch(...){
		setFailed();
		if(params->isV()){
			printf("  !!!!! %ls [%ls at %d]... failed in teardown()\n", this->name->towString(), this->file->towString(), getLine());
		}
		return;
	}

	if(params->isV()){
		if(isFailed()){
			result = "Failed!!!!!!!!!!!";
		}else{
			result = "OK";
		}
		double milli = ((double)this->microsec) / (double)1000;
		printf("  %ls() [%ls at %d]... %s(%lf ms)\n", this->name->towString(), this->file->towString(), getLine(), result, milli);
	}
	else{
		printf(".");
		fflush( stdout );
	}
}

Check* TestCase::addCheck(Check* check) noexcept {
	this->checks->addElement(check);
	return check;
}


const TestGroup* TestCase::getGroup() const noexcept {
	return this->group;
}
TestEnv* TestCase::getEnv() noexcept {
	return this->env;
}
const UnicodeString* TestCase::getName() const noexcept {
	return this->name;
}

const UnicodeString* TestCase::getFile() const noexcept {
	return this->file;
}

const int TestCase::getLine() const noexcept {
	return this->line;
}

bool TestCase::isDone() const noexcept {
	return this->done;
}

void TestCase::setDone() noexcept {
	this->done = true;
}

bool TestCase::isFailed() const noexcept {
	return this->failed;
}

void TestCase::setFailed() noexcept {
	this->failed = true;
}

ArrayList<Check>* TestCase::getChecks() const noexcept {
	return this->checks;
}

void TestCase::exportJUnitXML(Writer* writer) const {
	char buff[255]{};
	double sec = ((double)this->microsec) / (double)1000000;
	::sprintf(buff, "%lf", sec);

	UnicodeString milStr(buff);

	UnicodeString caseStr(L"    <testcase classname=\"");
	caseStr.append(this->group->getName()).append(L"\" name=\"").append(this->name).append(L"\" time=\"")
			.append(&milStr).append(L"\">\n");
	writer->write(&caseStr);

	if(failed){
		writer->write(L"      <failure>\n");

		UnicodeString failure(L"");
		failure.append(this->file).append(L" at ").append(this->line).append(L"\n");
		writer->write(L"        ");
		writer->write(&failure);

		writer->write(L"      </failure>\n");
	}

	writer->write(L"    </testcase>\n");
}


} /* namespace alinous */

