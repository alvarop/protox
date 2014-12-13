#include <stdio.h>
#include <string.h>
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "protox.h"
#include "a7105.h"

#define RSSI_SAMPLES (15)

typedef struct {
	uint8_t packetType;
	uint8_t unknown1;
	uint8_t throttle;
	uint8_t unknown2;
	uint8_t yaw;
	uint8_t unknown3;
	uint8_t pitch;
	uint8_t unknown4;
	uint8_t roll;
	uint8_t flips;
	uint8_t unknown5;
	uint8_t unknown6;
	uint8_t unknown7;
	uint8_t unknown8;
	uint8_t unknown9;
} __attribute__((packed)) controlPacket_t;

typedef enum {IDLE, SEND_PACKET, WAIT_FOR_REPLY, RUNNING} remoteState_t;
typedef enum {COUNT1, COUNT2, COUNT3} connectState_t;

static const uint8_t channels[] = {0x14, 0x1e, 0x28, 0x32, 0x3c, 0x46, 0x50, 0x5a, 0x64, 0x6e, 0x78, 0x82};
static uint8_t magicalPacket[] = {0x01, 0x82, 0xC0, 0xAC, 0xD8, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static controlPacket_t controlPacket = {0x20, 0x00, 0x00, 0x00, 0x80, 0x00, 0x6C, 0x00, 0x80, 0x06, 0x19, 0x00, 0x00, 0x00, 0x00};

static connectState_t state = COUNT1;
static remoteState_t remoteState = IDLE;
static uint8_t savedBestChannel = 0;
static uint8_t packetBuff[256];

extern volatile uint32_t tickMs;
static uint32_t rssiValues[sizeof(channels)];

static void msDelay(uint32_t delay) {
	uint32_t finishTime = tickMs + delay;

	while(tickMs < finishTime) {
		__WFI();
	}
}

static uint8_t protoXChecksum(uint8_t *buff, uint8_t len) {
	uint8_t checksum = 0;

	for(uint8_t x = 0; x < len; x++) {
		checksum -= buff[x];
	}

	return checksum;
}

static uint8_t getBestChannel() {
	uint8_t bestChannel = 0;

	a7105SetChannel(0xA0);
	a7105Strobe(STROBE_RX);

	a7105WriteReg(ADC_CTL, 0xC3);
	for(uint8_t channel = 0; channel < sizeof(channels); channel++) {
		a7105SetChannel(channels[channel]);
		a7105Strobe(STROBE_RX);

		msDelay(5);

		rssiValues[channel] = 0;

		for(uint8_t sample = 0; sample < RSSI_SAMPLES; sample++) {
			rssiValues[channel] += a7105ReadReg(RSSI_THRESHOLD);
			msDelay(2);
		}

		printf("Ch 0x%02X - RSSI total: %ld\n", channels[channel], rssiValues[channel]);

		if(rssiValues[channel] <  rssiValues[bestChannel]) {
			bestChannel = channel;
		}
	}

	printf("Best channel is: %02X, setting\n", channels[bestChannel]);
	a7105SetChannel(channels[bestChannel]);

	msDelay(5);

	return bestChannel;
}

void protoXInit() {
	a7105Init();
	savedBestChannel = getBestChannel();
}

void protoXSendPacket(uint8_t *buff, uint8_t len) {
	memcpy(packetBuff, buff, len);
	packetBuff[len] = protoXChecksum(packetBuff, len);

	a7105Strobe(STROBE_STANDBY);
	a7105Strobe(STROBE_FIFO_WR_RST);
	a7105Write(FIFO_DATA, packetBuff, len + 1);
	a7105Strobe(STROBE_TX);

	while (a7105ReadReg(MODE) & MODE_TRER);
}

int32_t processPacket(uint8_t *packet) {
	int32_t rval = 0;

	if(protoXChecksum(packet, 16) == 0) {
		switch(state) {
			case COUNT1: {
				if(packet[0] == 4) {
					magicalPacket[0] = 1;

					// Set the new ID
					a7105Write(ID_DATA, &magicalPacket[2], 4);

					state = COUNT2;
				} else {
					magicalPacket[0] = packet[0] + 1;
				}
				break;
			}

			case COUNT2: {
				magicalPacket[0] = 0x9;
				magicalPacket[2] = 0x00;
				state = COUNT3;
				break;
			}

			case COUNT3: {
				if(packet[1] == 9) {
					magicalPacket[0] = 1;	
					state = COUNT1;

					// Enable CRC
					a7105WriteReg(CODE_I, 0x0F);
					rval = 1;
				} else {
					magicalPacket[2] = packet[1];
				}
				break;
			}
		}
		
	} else {
		printf("Bad packet! [");
		for(uint8_t x = 0; x < 16; x++) {
			printf("%02X ", packet[x]);
		}
		printf("]\n");
		rval = 1;
	}

	return rval;
}

void protoXRemoteStart() {
	remoteState = SEND_PACKET;
}

void protoXSetThrottle(uint8_t throttle) {
	controlPacket.throttle = throttle;
}

void protoXSetPitch(uint8_t pitch) {
	controlPacket.pitch = pitch;
}

void protoXSetYaw(uint8_t yaw) {
	controlPacket.yaw = yaw;
}

void protoXSetRoll(uint8_t roll) {
	controlPacket.roll = roll;
}

int32_t protoXProcess() {

	static uint32_t timeout = 0;
	static uint32_t timeToCHSwitch = 0;
	static uint32_t channelRefresh = 0;
	uint32_t stopWFI = 0;

	switch(remoteState) {
		case IDLE: {
			stopWFI = 0;
			break;
		}

		case SEND_PACKET: {

			protoXSendPacket(magicalPacket, 15);

			a7105Strobe(STROBE_RX);

			timeout = tickMs + 14;

			remoteState = WAIT_FOR_REPLY;

			// Don't let the processor go to sleep while we're pairing
			stopWFI = 1;

			break;
		}

		case WAIT_FOR_REPLY: {

			// Heard it!
			if ((a7105ReadReg(MODE) & MODE_TRER) == 0) {
				a7105Strobe(STROBE_FIFO_RD_RST);
				a7105Read(FIFO_DATA, packetBuff, 16);
				
				if(processPacket(packetBuff)) {
					remoteState = RUNNING;
					timeToCHSwitch = tickMs + 50;
					timeout = tickMs;
				} else {
					remoteState = SEND_PACKET;
				}
			} else if(tickMs >= timeout) {
				remoteState = SEND_PACKET;
			}

			// Don't let the processor go to sleep while we're pairing
			stopWFI = 1;

			break;
		}

		case RUNNING: {

			if(tickMs >= timeout){
				timeout = tickMs + 10;

				if(channelRefresh) {
					channelRefresh = 0;
					a7105SetChannel(channels[savedBestChannel]);
					timeToCHSwitch = tickMs + 50;
				} else if(tickMs >= timeToCHSwitch) {
					a7105SetChannel(0xA5);
					channelRefresh = 1;
				}

				protoXSendPacket((uint8_t *)&controlPacket, 15);
			}

			// We're dealing with 10 ms timings, so waking up on systick should be ok
			stopWFI = 0;

			break;
		}
	}

	return stopWFI;
}
