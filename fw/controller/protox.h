#include <stdint.h>
#include <stdbool.h>

void protoXInit();
int32_t protoXProcess();

void protoXSnifferStart();
void protoXSetCh(uint8_t ch);
void protoXSetId(uint8_t *id);
void protoXCRCEnable(bool enabled);
void protoXFindRemote();

void protoXRemoteStart();
void protoXRemoteForceStart();
void protoXRemoteDOS();
void protoXSetThrottle(uint8_t throttle);
void protoXSetPitch(uint8_t pitch);
void protoXSetYaw(uint8_t yaw);
void protoXSetRoll(uint8_t roll);