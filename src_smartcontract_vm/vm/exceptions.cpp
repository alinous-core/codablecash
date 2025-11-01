/*
 * exceptions.cpp
 *
 *  Created on: 2019/04/14
 *      Author: iizuka
 */

#include "vm/exceptions.h"
#include "base/UnicodeString.h"

namespace alinous {

const wchar_t* VmMemoryAllocationException::defaultMessage = L"FileStorage error. ";

VmMemoryAllocationException::VmMemoryAllocationException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
VmMemoryAllocationException::VmMemoryAllocationException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
VmMemoryAllocationException::VmMemoryAllocationException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
VmMemoryAllocationException::VmMemoryAllocationException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
VmMemoryAllocationException::~VmMemoryAllocationException() {
}

//////////////////////////////////////////////////////////////////////////////////////////
const wchar_t* VmMethodNotFoundException::defaultMessage = L"Method is not found. ";

VmMethodNotFoundException::VmMethodNotFoundException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
VmMethodNotFoundException::VmMethodNotFoundException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
VmMethodNotFoundException::VmMethodNotFoundException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
VmMethodNotFoundException::VmMethodNotFoundException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
VmMethodNotFoundException::~VmMethodNotFoundException() {
}

//////////////////////////////////////////////////////////////////////////////////////////
const wchar_t* VmClassNotFoundException::defaultMessage = L"Class is not found. ";

VmClassNotFoundException::VmClassNotFoundException(const char* srcfile, int srcline) noexcept : Exception(srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
VmClassNotFoundException::VmClassNotFoundException(Exception* cause, const char* srcfile, int srcline) noexcept : Exception(cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
}
VmClassNotFoundException::VmClassNotFoundException(const wchar_t* message, const char* srcfile, int srcline) noexcept : Exception(message, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
VmClassNotFoundException::VmClassNotFoundException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept : Exception(message, cause, srcfile, srcline) {
	this->message = new UnicodeString(defaultMessage);
	this->message->append(message);
}
VmClassNotFoundException::~VmClassNotFoundException() {
}

} /* namespace alinous */
