/*
 * Base64.cpp
 *
 *  Created on: 2023/02/22
 *      Author: iizuka
 */

#include "bc_base/Base64.h"

#include "base/UnicodeString.h"

#include "base_io/ByteBuffer.h"

#include "osenv/memory.h"

namespace codablecash {

UnicodeString* Base64::encode(const uint8_t* data, int len) {
	int start = 0;
	UnicodeString *buf = new UnicodeString(L"");

	int end = len - 3;
	int i = start;
	int n = 0;

	while (i <= end) {
		int d = ((((int) data[i]) & 0x0ff) << 16)
				| ((((int) data[i + 1]) & 0x0ff) << 8)
				| (((int) data[i + 2]) & 0x0ff);
		buf->append(legalChars[(d >> 18) & 63]);
		buf->append(legalChars[(d >> 12) & 63]);
		buf->append(legalChars[(d >> 6) & 63]);
		buf->append(legalChars[d & 63]);
		i += 3;
		if (n++ >= 14) {
			n = 0;
			buf->append(L" ");
		}
	}

    if(i == start + len - 2){
        int d = ((((int) data[i]) & 0x0ff) << 16)
                | ((((int) data[i + 1]) & 255) << 8);
        buf->append(legalChars[(d >> 18) & 63]);
        buf->append(legalChars[(d >> 12) & 63]);
        buf->append(legalChars[(d >> 6) & 63]);
        buf->append(L"=");
    }
    else if(i == (start + len - 1) ){
        int d = (((int) data[i]) & 0x0ff) << 16;
        buf->append(legalChars[(d >> 18) & 63]);
        buf->append(legalChars[(d >> 12) & 63]);
        buf->append(L"==");
    }

    return buf;
}
/*
private static final int[] IA = new int[256];
static {
	Arrays.fill(IA, -1);
	for (int i = 0, iS = CA.length; i < iS; i++)
		IA[CA[i]] = i;
	IA['='] = 0;
}*/
ByteBuffer* Base64::decode(const char *sArr, int sLen) {
	const int *IA = getIA();

	int sepCnt = 0; // Number of separator characters. (Actually illegal characters, but that's a bonus...)
	for (int i = 0; i < sLen; i++) // If input is "pure" (I.e. no line separators or illegal chars) base64 this loop can be commented out.
		if (IA[sArr[i]] < 0)
			sepCnt++;

	assert((sLen - sepCnt) % 4 == 0);

	int pad = 0;
	for (int i = sLen; i > 1 && IA[sArr[--i]] <= 0;)
		if (sArr[i] == '=')
			pad++;

	int len = ((sLen - sepCnt) * 6 >> 3) - pad;

	ByteBuffer* dArr = ByteBuffer::allocateWithEndian(len, true);       // Preallocate byte[] of exact length

	for (int s = 0, d = 0; d < len;) {
		// Assemble three bytes into an int from four "valid" characters.
		int i = 0;
		for (int j = 0; j < 4; j++) { // j only increased if a valid char was found.
			int c = IA[sArr[s++]];
			if (c >= 0)
				i |= c << (18 - j * 6);
			else
				j--;
		}
		// Add the bytes
		//dArr[d++] = (byte)(i >> 16);
		dArr->put(d++, i >> 16);

		if (d < len) {
			//dArr[d++] = (byte)(i >> 8);
			dArr->put(d++, i >> 8);

			if (d < len){
				// dArr[d++] = (byte) i;
				dArr->put(d++, i);
			}
		}
	}

	return dArr;
}

const int* Base64::getIA() {
	static int IA[256];
	static bool init = false;

	if(!init){
		Mem::memset(IA, -1, sizeof(IA));
		for (int i = 0; i < 64; i++){
			IA[legalChars[i]] = i;
		}
		IA['='] = 0;

		init = true;
	}

	return IA;
}

} /* namespace codablecash */
