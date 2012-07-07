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

#include <stdlib.h>

#include "IDxSerial.h"

IDxSerial::IDxSerial(uint8_t rxPin, uint8_t txPin) {
	bus = new SoftwareSerial(rxPin, txPin);
	bus->begin(9600);
}

bool IDxSerial::hasData(void) {
	return (bus->available() > 0);
}

IDxStatus IDxSerial::getData(byte ** code) {
	byte checksum = 0, value = 0, temp = 0, bytesread = 0, data[6];

	if (!hasData()) {
		return IDx_NO_DATA;
	}

	value = bus->read();

	// If byte read is STX, we have a valid header byte. Continue reading.
	if (value == ASCII_STX) {
		while (true) {
			value = bus->read();

			// Another header byte, this should not happen.
			if (value == ASCII_STX) {
				break;
			}

			// Closing byte. End of message reached.
			if (value == ASCII_ETX) {
				break;
			}

			// Line terminator is part of the message, but not important.
			if (value == ASCII_LF || value == ASCII_CR) {
				continue;
			}

			// Data is written in ASCII, convert HEX code to actual value.
			if (value >= '0' && value <= '9') {
				value -= '0';
			}
			else if (value >= 'A' && value <= 'F') {
				value += 10;
				value -= 'A';
			}
			// Invalid data, discard and continue.
			else {
				continue;
			}

			// At every second byte read, combine with previous read byte to get the actual value.
			if ((bytesread & 1) == 1) {
				data[bytesread >> 1] = value | (temp << 4);

				// Calculate the checksum by XORing the byte value. The sixth byte is the checksum.
				if ((bytesread >> 1) < 5) {
					checksum ^= data[bytesread >> 1];
				}
			}
			// Store byte for next cycle.
			else {
				temp = value;
			}

			bytesread++;
		}
	}
	else {
		return IDx_INVALID_DATA;
	}

	if (bytesread < 12) {
		return IDx_INCOMPLETE;
	}

	if (data[6] != checksum) {
		return IDx_CHECKSUM_FAILED;
	}

	if (bytesread == 12) {
		*code = malloc(sizeof(byte) * 5);
		*code = memcpy(*code, data, sizeof(byte) * 5);

		return IDx_VALID_CODE;
	}

	return IDx_ERROR;
}
