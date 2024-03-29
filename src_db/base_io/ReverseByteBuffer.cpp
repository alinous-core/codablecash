/*
 * ReverseByteBuffer.cpp
 *
 *  Created on: 2018/11/02
 *      Author: iizuka
 */

#include "base_io/ReverseByteBuffer.h"
#include "base/RawArrayPrimitive.h"

#include "base_io/exceptions.h"

namespace alinous {


ReverseByteBuffer::ReverseByteBuffer(const int length) noexcept : ByteBuffer(length){
}

ReverseByteBuffer::ReverseByteBuffer(const uint8_t* buffer, int length) : ByteBuffer(buffer, length) {
}

ReverseByteBuffer::~ReverseByteBuffer() noexcept {
}

ByteBuffer* ReverseByteBuffer::wrap(const uint8_t* buffer, int length) {
	ReverseByteBuffer* buff = new ReverseByteBuffer(buffer, length);
	return buff;
}



ByteBuffer* ReverseByteBuffer::putChar(wchar_t value) noexcept(false) {
	if(remaining() < (int)sizeof(short)){
		throw new BufferOverflowException(L"put(wchar_t value)", __FILE__, __LINE__);
	}

	//data->set(this->pos++, value & 0xFF);
	//data->set(this->pos++, (value >> 1) & 0xFF);

	uint16_t sh = (uint16_t)value;
	char* bytes = (char*)&sh;

	data->set(this->pos++, bytes[1]);
	data->set(this->pos++, bytes[0]);

	return this;
}

ByteBuffer* ReverseByteBuffer::putChar(int position, wchar_t value) noexcept(false) {
	if(this->lim - position < (int)sizeof(short)){
		throw new BufferOverflowException(L"putChar(int position, wchar_t value)", __FILE__, __LINE__);
	}

	//data->set(this->pos++, value & 0xFF);
	//data->set(this->pos++, (value >> 1) & 0xFF);

	uint16_t sh = (uint16_t)value;
	char* bytes = (char*)&sh;

	data->set(position++, bytes[1]);
	data->set(position++, bytes[0]);

	return this;
}

ByteBuffer* ReverseByteBuffer::putShort(short value) noexcept(false) {
	if(remaining() < (int)sizeof(short)){
		throw new BufferOverflowException(L"putShort(short value)", __FILE__, __LINE__);
	}

	char* bytes = (char*)&value;

	data->set(this->pos++, bytes[1]);
	data->set(this->pos++, bytes[0]);

	return this;
}

ByteBuffer* ReverseByteBuffer::putShort(int position, short value) noexcept(false) {
	if(this->lim - position < (int)sizeof(short)){
		throw new BufferOverflowException(L"putShort(int position, short value)", __FILE__, __LINE__);
	}

	char* bytes = (char*)&value;

	this->pos = position;
	data->set(this->pos++, bytes[1]);
	data->set(this->pos++, bytes[0]);

	return this;
}

ByteBuffer* ReverseByteBuffer::putInt(int32_t value) noexcept(false) {
	if(remaining() < (int)sizeof(int32_t)){
		throw new BufferOverflowException(L"putInt(int32_t value)", __FILE__, __LINE__);
	}

	char* bytes = (char*)&value;

	data->set(this->pos++, bytes[3]);
	data->set(this->pos++, bytes[2]);
	data->set(this->pos++, bytes[1]);
	data->set(this->pos++, bytes[0]);

	return this;
}

ByteBuffer* ReverseByteBuffer::putInt(int32_t position, int value) noexcept(false) {
	if(this->lim - position < (int)sizeof(int32_t)){
		throw new BufferOverflowException(L"putInt(int32_t position, int value)", __FILE__, __LINE__);
	}

	char* bytes = (char*)&value;

	this->pos = position;
	data->set(this->pos++, bytes[3]);
	data->set(this->pos++, bytes[2]);
	data->set(this->pos++, bytes[1]);
	data->set(this->pos++, bytes[0]);

	return this;
}

ByteBuffer* ReverseByteBuffer::putLong(int64_t value) noexcept(false) {
	if(remaining() < (int)sizeof(int64_t)){
		throw new BufferOverflowException(L"putLong(int64_t value)", __FILE__, __LINE__);
	}

	char* bytes = (char*)&value;

	data->set(this->pos++, bytes[7]);
	data->set(this->pos++, bytes[6]);
	data->set(this->pos++, bytes[5]);
	data->set(this->pos++, bytes[4]);
	data->set(this->pos++, bytes[3]);
	data->set(this->pos++, bytes[2]);
	data->set(this->pos++, bytes[1]);
	data->set(this->pos++, bytes[0]);

	return this;
}

ByteBuffer* ReverseByteBuffer::putLong(int position, int64_t value) noexcept(false) {
	if(this->lim - position < (int)sizeof(int64_t)){
		throw new BufferOverflowException(L"putLong(int position, int64_t value)", __FILE__, __LINE__);
	}

	char* bytes = (char*)&value;

	this->pos = position;
	data->set(this->pos++, bytes[7]);
	data->set(this->pos++, bytes[6]);
	data->set(this->pos++, bytes[5]);
	data->set(this->pos++, bytes[4]);
	data->set(this->pos++, bytes[3]);
	data->set(this->pos++, bytes[2]);
	data->set(this->pos++, bytes[1]);
	data->set(this->pos++, bytes[0]);

	return this;
}

ByteBuffer* ReverseByteBuffer::putFloat(float value) noexcept(false) {
	if(remaining() < (int)sizeof(float)){
		throw new BufferOverflowException(L"putFloat(float value)", __FILE__, __LINE__);
	}

	char* bytes = (char*)&value;

	data->set(this->pos++, bytes[3]);
	data->set(this->pos++, bytes[2]);
	data->set(this->pos++, bytes[1]);
	data->set(this->pos++, bytes[0]);

	return this;
}

ByteBuffer* ReverseByteBuffer::putFloat(int position, float value) noexcept(false) {
	if(this->lim - position < (int)sizeof(float)){
		throw new BufferOverflowException(L"putFloat(int position, float value)", __FILE__, __LINE__);
	}

	char* bytes = (char*)&value;

	this->pos = position;
	data->set(this->pos++, bytes[3]);
	data->set(this->pos++, bytes[2]);
	data->set(this->pos++, bytes[1]);
	data->set(this->pos++, bytes[0]);

	return this;
}

ByteBuffer* ReverseByteBuffer::putDouble(double value) noexcept(false) {
	if(remaining() < (int)sizeof(double)){
		throw new BufferOverflowException(L"putDouble(double value)", __FILE__, __LINE__);
	}

	char* bytes = (char*)&value;

	data->set(this->pos++, bytes[7]);
	data->set(this->pos++, bytes[6]);
	data->set(this->pos++, bytes[5]);
	data->set(this->pos++, bytes[4]);
	data->set(this->pos++, bytes[3]);
	data->set(this->pos++, bytes[2]);
	data->set(this->pos++, bytes[1]);
	data->set(this->pos++, bytes[0]);

	return this;
}

ByteBuffer* ReverseByteBuffer::putDouble(int position, double value) noexcept(false) {
	if(this->lim - position < (int)sizeof(double)){
		throw new BufferOverflowException(L"putDouble(int position, double value)", __FILE__, __LINE__);
	}

	char* bytes = (char*)&value;

	this->pos = position;
	data->set(this->pos++, bytes[7]);
	data->set(this->pos++, bytes[6]);
	data->set(this->pos++, bytes[5]);
	data->set(this->pos++, bytes[4]);
	data->set(this->pos++, bytes[3]);
	data->set(this->pos++, bytes[2]);
	data->set(this->pos++, bytes[1]);
	data->set(this->pos++, bytes[0]);

	return this;
}

int64_t ReverseByteBuffer::getLong() noexcept(false) {
	if(remaining() < (int)sizeof(int64_t)){
		throw new BufferOverflowException(__FILE__, __LINE__);
	}

	uint8_t* ptr = this->data->getRoot() + this->pos;
	uint8_t dest[8]{};

	dest[0] = ptr[7];
	dest[1] = ptr[6];
	dest[2] = ptr[5];
	dest[3] = ptr[4];
	dest[4] = ptr[3];
	dest[5] = ptr[2];
	dest[6] = ptr[1];
	dest[7] = ptr[0];

	this->pos += sizeof(uint64_t);

	return *(reinterpret_cast<int64_t*>((void*)dest));
}

int64_t ReverseByteBuffer::getLong(int position) noexcept(false) {
	if(this->lim - position < (int)sizeof(int64_t)){
		throw new BufferOverflowException(__FILE__, __LINE__);
	}

	uint8_t* ptr = this->data->getRoot() + position;
	uint8_t dest[8]{};

	dest[0] = ptr[7];
	dest[1] = ptr[6];
	dest[2] = ptr[5];
	dest[3] = ptr[4];
	dest[4] = ptr[3];
	dest[5] = ptr[2];
	dest[6] = ptr[1];
	dest[7] = ptr[0];

	this->pos = position + sizeof(uint64_t);

	return *(reinterpret_cast<int64_t*>((void*)dest));
}

double ReverseByteBuffer::getDouble() noexcept(false) {
	if(remaining() < (int)sizeof(double)){
		throw new BufferOverflowException(__FILE__, __LINE__);
	}

	uint8_t* ptr = this->data->getRoot() + this->pos;
	uint8_t dest[8]{};

	dest[0] = ptr[7];
	dest[1] = ptr[6];
	dest[2] = ptr[5];
	dest[3] = ptr[4];
	dest[4] = ptr[3];
	dest[5] = ptr[2];
	dest[6] = ptr[1];
	dest[7] = ptr[0];

	this->pos += sizeof(double);

	return *(reinterpret_cast<double*>((void*)dest));
}

double ReverseByteBuffer::getDouble(int position) noexcept(false) {
	if(this->lim - position < (int)sizeof(double)){
		throw new BufferOverflowException(__FILE__, __LINE__);
	}

	uint8_t* ptr = this->data->getRoot() + position;
	uint8_t dest[8]{};

	dest[0] = ptr[7];
	dest[1] = ptr[6];
	dest[2] = ptr[5];
	dest[3] = ptr[4];
	dest[4] = ptr[3];
	dest[5] = ptr[2];
	dest[6] = ptr[1];
	dest[7] = ptr[0];

	this->pos = position + sizeof(double);

	return *(reinterpret_cast<double*>((void*)dest));
}

float ReverseByteBuffer::getFloat() noexcept(false) {
	if(remaining() < (int)sizeof(float)){
		throw new BufferOverflowException(__FILE__, __LINE__);
	}

	uint8_t* ptr = this->data->getRoot() + this->pos;
	uint8_t dest[4]{};

	dest[0] = ptr[3];
	dest[1] = ptr[2];
	dest[2] = ptr[1];
	dest[3] = ptr[0];

	this->pos += sizeof(float);

	return *(reinterpret_cast<float*>((void*)dest));
}

float ReverseByteBuffer::getFloat(int position) noexcept(false) {
	if(this->lim - position < (int)sizeof(float)){
		throw new BufferOverflowException(__FILE__, __LINE__);
	}

	uint8_t* ptr = this->data->getRoot() + position;
	uint8_t dest[4]{};

	dest[0] = ptr[3];
	dest[1] = ptr[2];
	dest[2] = ptr[1];
	dest[3] = ptr[0];

	this->pos = position + sizeof(float);

	return *(reinterpret_cast<float*>((void*)dest));
}

int32_t ReverseByteBuffer::getInt() noexcept(false) {
	if(remaining() < (int)sizeof(int32_t)){
		throw new BufferOverflowException(L"", __FILE__, __LINE__);
	}

	char out[4]{};
	char* bytes = (char*)(this->data->getRoot() + this->pos);

	out[0] = bytes[3];
	out[1] = bytes[2];
	out[2] = bytes[1];
	out[3] = bytes[0];

	wchar_t val = *((int32_t*)out);
	this->pos += sizeof(int32_t);

	return val;
}

int32_t ReverseByteBuffer::getInt(int position) noexcept(false) {
	if(this->lim - position < (int)sizeof(int32_t)){
		throw new BufferOverflowException(__FILE__, __LINE__);
	}

	char out[4]{};
	char* bytes = (char*)(this->data->getRoot() + position);

	out[0] = bytes[3];
	out[1] = bytes[2];
	out[2] = bytes[1];
	out[3] = bytes[0];

	wchar_t val = *((int32_t*)out);
	this->pos = position + sizeof(int32_t);

	return val;
}

wchar_t ReverseByteBuffer::getChar() noexcept(false) {
	if(remaining() < (int)sizeof(int16_t)){
		throw new BufferOverflowException(L"", __FILE__, __LINE__);
	}

	char out[2]{};
	char* bytes = (char*)(this->data->getRoot() + this->pos);

	out[0] = bytes[1];
	out[1] = bytes[0];

	wchar_t val = *((short*)out);
	this->pos += sizeof(int16_t);

	return val;
}

wchar_t ReverseByteBuffer::getChar(int position) noexcept(false) {
	if(this->lim - position < (int)sizeof(int16_t)){
		throw new BufferOverflowException(__FILE__, __LINE__);
	}

	char out[2]{};
	char* bytes = (char*)(this->data->getRoot() + position);

	out[0] = bytes[1];
	out[1] = bytes[0];

	wchar_t val = *((short*)out);
	this->pos = position + sizeof(int16_t);

	return val;
}

int16_t ReverseByteBuffer::getShort() noexcept(false) {
	if(remaining() < (int)sizeof(int16_t)){
		throw new BufferOverflowException(L"", __FILE__, __LINE__);
	}

	char out[2]{};
	char* bytes = (char*)(this->data->getRoot() + this->pos);

	out[0] = bytes[1];
	out[1] = bytes[0];

	short val = *((short*)out);
	this->pos += sizeof(int16_t);

	return val;
}

int16_t ReverseByteBuffer::getShort(int position) noexcept(false) {
	if(this->lim - position < (int)sizeof(int16_t)){
		throw new BufferOverflowException(__FILE__, __LINE__);
	}
	char out[2]{};
	char* bytes = (char*)(this->data->getRoot() + position);

	out[0] = bytes[1];
	out[1] = bytes[0];

	short val = *((short*)out);
	this->pos = position + sizeof(int16_t);

	return val;
}

ByteBuffer* ReverseByteBuffer::clone() const noexcept(false) {
	return new ReverseByteBuffer(array(), this->cap);
}
} /* namespace alinous */


