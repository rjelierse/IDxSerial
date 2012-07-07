/**
 * IDxSerial library for Arduino
 *
 * Copyright 2012 Raymond Jelierse
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IDxSerial_h
#define IDxSerial_h

#include <SoftwareSerial.h>

#define ASCII_STX 0x02 // Start of text
#define ASCII_ETX 0x03 // End of text
#define ASCII_LF  0x0A // Line feed
#define ASCII_CR  0x0D // Carriage return

typedef enum _IDxStatus {
	IDx_VALID_CODE,
	IDx_CHECKSUM_FAILED,
	IDx_INCOMPLETE,
	IDx_INVALID_DATA,
	IDx_NO_DATA,
	IDx_ERROR
} IDxStatus;

class IDxSerial
{
private:
	SoftwareSerial *bus;
public:
	IDxSerial(uint8_t, uint8_t);
	bool hasData(void);
	IDxStatus getData(byte **);
};

#endif /* IDxSerial_h */
