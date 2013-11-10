/*
 * Copyright (C) 2013 Florian Echtler <floe@butterbrot.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation.
*/

#include <Arduino.h>
#include "./BTLE.h"

uint8_t swapbits(uint8_t a) {

	uint8_t v = 0;

	if (a & 0x80) v |= 0x01;
	if (a & 0x40) v |= 0x02;
	if (a & 0x20) v |= 0x04;
	if (a & 0x10) v |= 0x08;
	if (a & 0x08) v |= 0x10;
	if (a & 0x04) v |= 0x20;
	if (a & 0x02) v |= 0x40;
	if (a & 0x01) v |= 0x80;

	return v;
}

// see BT Core Spec 4.0, Section 6.B.3.1.1
void btLeCrc(const uint8_t* data, uint8_t len, uint8_t* dst) {

	uint8_t v, t, d;

	while (len--) {

		d = *data++;
		for (v = 0; v < 8; v++, d >>= 1) {

			// t = bit 23 (highest-value) 
			t = dst[0] >> 7;

			// left-shift the entire register by one
			// (dst[0] = bits 23-16, dst[1] = bits 15-8, dst[2] = bits 7-0
			dst[0] <<= 1;
			if(dst[1] & 0x80) dst[0] |= 1;
			dst[1] <<= 1;
			if(dst[2] & 0x80) dst[1] |= 1;
			dst[2] <<= 1;

			// if the bit just shifted out (former bit 23)
			// and the incoming data bit are not equal:
			// => bit_out ^ bit_in = 1
			if (t != (d & 1)) {
				// toggle register bits (=XOR with 1) according to CRC polynom
				dst[2] ^= 0x5B; // 0b01011011 - x^6+x^4+x^3+x+1
				dst[1] ^= 0x06; // 0b00000110 - x^10+x^9
			}
		}
	}
}

// see BT Core Spec 4.0, Section 6.B.3.2
void btLeWhiten(uint8_t* data, uint8_t len, uint8_t whitenCoeff) {

	uint8_t  m;

	while (len--) {

		for (m = 1; m; m <<= 1) {

			if (whitenCoeff & 0x80) {
 
				whitenCoeff ^= 0x11;
				(*data) ^= m;
			}
			whitenCoeff <<= 1;
		}
		data++;
	}
}


static inline uint8_t btLeWhitenStart(uint8_t chan) {
	//the value we actually use is what BT'd use left shifted one...makes our life easier
	return swapbits(chan) | 2;
}


void btLePacketEncode(uint8_t* packet, uint8_t len, uint8_t chan) {
	//length is of packet, including crc. pre-populate crc in packet with initial crc value!
	uint8_t i, dataLen = len - 3;

	btLeCrc(packet, dataLen, packet + dataLen);
	for (i = 0; i < 3; i++, dataLen++) packet[dataLen] = swapbits(packet[dataLen]);
	btLeWhiten(packet, len, btLeWhitenStart(chan));
	for (i = 0; i < len; i++) packet[i] = swapbits(packet[i]);
}


const uint8_t channel[3]   = {37,38,39};  // logical BTLE channel number (37-39)
const uint8_t frequency[3] = { 2,26,80};  // physical frequency (2400+x MHz)


// This is a rather convoluted hack to extract the month number from the build date in
// the __DATE__ macro using a small hash function + lookup table. Since all inputs are
// const, this can be fully resolved by the compiler and saves over 200 bytes of code.
#define month(m) month_lookup[ (( ((( (m[0] % 24) * 13) + m[1]) % 24) * 13) + m[2]) % 24 ]
const uint8_t month_lookup[24] = { 0,6,0,4,0,1,0,17,0,8,0,0,3,0,0,0,18,2,16,5,9,0,1,7 };


// constructor
BTLE::BTLE( RF24* _radio ):
	radio(_radio),
	current(0)
{ }

// set BTLE-compatible radio parameters
void BTLE::begin( const char* _name ) {

	name = _name;
	radio->begin();

	// set standard parameters
	radio->setAutoAck(false);
	radio->setDataRate(RF24_1MBPS);
	radio->disableCRC();
	radio->setChannel( frequency[current] );
	radio->setRetries(0,0);
	radio->setPALevel(RF24_PA_MAX);

	// set advertisement address: 0x8E89BED6 (bit-reversed -> 0x6B7D9171)
	radio->setAddressSize(4);
	radio->openReadingPipe(0,0x6B7D9171);
	radio->openWritingPipe(  0x6B7D9171);

	radio->powerUp();
}

/* // set a specific MAC address
void BTLE::setMAC( uint8_t buf[6] ) {
	for (int i = 0; i < 6; i++)
		outbuf[i+2] = buf[i];
}

// set pseudo-random MAC derived from build date
void BTLE::setBuildMAC() {
	outbuf[2] = ((__TIME__[6]-0x30) << 4) | (__TIME__[7]-0x30);
	outbuf[3] = ((__TIME__[3]-0x30) << 4) | (__TIME__[4]-0x30);
	outbuf[4] = ((__TIME__[0]-0x30) << 4) | (__TIME__[1]-0x30);
	outbuf[5] = ((__DATE__[4]-0x30) << 4) | (__DATE__[5]-0x30);
	outbuf[6] = month(__DATE__);
	outbuf[7] = ((__DATE__[9]-0x30) << 4) | (__DATE__[10]-0x30);
} */

// set the current channel (from 36 to 38)
void BTLE::setChannel( uint8_t num ) {
	current = min(2,max(0,num-37));
	radio->setChannel( frequency[current] );
}

// hop to the next channel
void BTLE::hopChannel() {
	current++;
	if (current >= sizeof(channel)) current = 0;
	radio->setChannel( frequency[current] );
}

// broadcast an advertisement packet
bool BTLE::advertise( void* buf, uint8_t buflen ) {

	// name & total payload size
	uint8_t namelen = strlen(name);
	uint8_t pls = 0;

	// insert pseudo-random MAC address
	buffer.mac[0] = ((__TIME__[6]-0x30) << 4) | (__TIME__[7]-0x30);
	buffer.mac[1] = ((__TIME__[3]-0x30) << 4) | (__TIME__[4]-0x30);
	buffer.mac[2] = ((__TIME__[0]-0x30) << 4) | (__TIME__[1]-0x30);
	buffer.mac[3] = ((__DATE__[4]-0x30) << 4) | (__DATE__[5]-0x30);
	buffer.mac[4] = month(__DATE__);
	buffer.mac[5] = ((__DATE__[9]-0x30) << 4) | (__DATE__[10]-0x30);

	// add device descriptor chunk
	chunk(buffer,pls)->size = 0x02;  // chunk size: 2
	chunk(buffer,pls)->type = 0x01;  // chunk type: device flags
	chunk(buffer,pls)->data[0]= 0x05;  // flags: LE-only, limited discovery mode
	pls += 3;

	// add "complete name" chunk
	chunk(buffer,pls)->size = namelen+1;  // chunk size
	chunk(buffer,pls)->type = 0x09;       // chunk type
	for (uint8_t i = 0; i < namelen; i++)
		chunk(buffer,pls)->data[i] = name[i];
	pls += namelen+2;

	// add custom data, if applicable
	if (buflen > 0) {
		chunk(buffer,pls)->size = buflen+1;  // chunk size
		chunk(buffer,pls)->type = 0xFF;      // chunk type
		for (uint8_t i = 0; i < buflen; i++)
			chunk(buffer,pls)->data[i] = ((uint8_t*)buf)[i];
		pls += buflen+2;
	}

	// add CRC placeholder
	buffer.payload[pls++] = 0x55;
	buffer.payload[pls++] = 0x55;
	buffer.payload[pls++] = 0x55;

	// total payload size must be 24 bytes or less
	if (pls > 24)
		return false;

	// assemble header
	buffer.pdu_type = 0x42;    // PDU type: ADV_NONCONN_IND, TX address is random
	buffer.pl_size = pls + 3;  // set final payload size in header incl. MAC excl. CRC

	// encode for current logical channel, flush buffers, send
	btLePacketEncode( (uint8_t*)&buffer, pls+8, channel[current] );
	radio->stopListening();
	radio->write( (uint8_t*)&buffer, pls+8 );

	return true;
}

// listen for advertisement packets
bool BTLE::listen() {

	radio->startListening();
	delay(20);

	if (!radio->available())
		return false;

	bool done = false;
	uint8_t total_size = 0;
	uint8_t* inbuf = (uint8_t*)&buffer;

	while (!done) {

		// fetch the payload, and check if there are more left
		done = radio->read( inbuf, sizeof(buffer) );

		// decode: swap bit order, un-whiten
		for (uint8_t i = 0; i < sizeof(buffer); i++) inbuf[i] = swapbits(inbuf[i]);
		btLeWhiten( inbuf, sizeof(buffer), btLeWhitenStart( channel[current] ) );
		
		// size is w/o header+CRC -> add 2 bytes header
		total_size = inbuf[1]+2;
		uint8_t crc[3] = { 0x55, 0x55, 0x55 };

		// calculate & compare CRC
		btLeCrc( inbuf, total_size, crc );
		for (uint8_t i = 0; i < 3; i++)
			if (inbuf[total_size+i] != swapbits(crc[i]))
				return false;
	}

	return true;
}

