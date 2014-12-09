#ifndef __A7105_H__
#define __A7105_H__

#include <stdint.h>

void a7105Init();
int32_t a7105Process();

void protoXRemoteStart();
void protoXSetThrottle(uint8_t throttle);
void protoXSetPitch(uint8_t pitch);
void protoXSetYaw(uint8_t yaw);
void protoXSetRoll(uint8_t roll);

#endif
