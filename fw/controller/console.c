#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "a7105.h"
#include "console.h"
#include "fifo.h"

typedef struct {
	char *commandStr;
	void (*fn)(uint8_t argc, char *argv[]);
	char *helpStr;
} command_t;

fifo_t usbRxFifo;

static char cmdBuff[64];
static uint8_t argc;
static char* argv[8];

static void helpFn(uint8_t argc, char *argv[]);
static void initRadio(uint8_t argc, char *argv[]);
static void remote(uint8_t argc, char *argv[]);

static command_t commands[] = {
	{"init", initRadio, "Initialize radio"},
	{"remote", remote, "Start the remote"},
	// Add new commands here!
	{"help", helpFn, "Print this!"},
	{NULL, NULL, NULL}
};

//
// Print the help menu
//
static void helpFn(uint8_t argc, char *argv[]) {
	command_t *command = commands;

	if(argc < 2) {
		while(command->commandStr != NULL) {
			printf("%s - %s\n", command->commandStr, command->helpStr);
			command++;
		}
	} else {
		while(command->commandStr != NULL) {
			if(strcmp(command->commandStr, argv[1]) == 0) {
				printf("%s - %s\n", command->commandStr, command->helpStr);
				break;
			}
			command++;
		}
	}
}

static void initRadio(uint8_t argc, char *argv[]) {
	a7105Init();
	protoXRemoteStart();
}

static void remote(uint8_t argc, char *argv[]) {
	switch(argc) {
		case 3: {
			switch(argv[1][0]) {
				case 't': {
					protoXSetThrottle((uint8_t)strtoul(argv[2], NULL, 10));
					break;
				}

				case 'p': {
					protoXSetPitch((uint8_t)strtoul(argv[2], NULL, 10));
					break;
				}

				case 'y': {
					protoXSetYaw((uint8_t)strtoul(argv[2], NULL, 10));
					break;
				}

				case 'r': {
					protoXSetRoll((uint8_t)strtoul(argv[2], NULL, 10));
					break;
				}
			}

			break;
		}

		case 5: {
			protoXSetThrottle((uint8_t)strtoul(argv[1], NULL, 10));
			protoXSetPitch((uint8_t)strtoul(argv[2], NULL, 10));
			protoXSetYaw((uint8_t)strtoul(argv[3], NULL, 10));
			protoXSetRoll((uint8_t)strtoul(argv[4], NULL, 10));
		}
	}

}


void consoleProcess() {
	uint32_t inBytes = fifoSize(&usbRxFifo);
	if(inBytes > 0) {
		uint32_t newLine = 0;
		for(int32_t index = 0; index < inBytes; index++){
			if((fifoPeek(&usbRxFifo, index) == '\n') || (fifoPeek(&usbRxFifo, index) == '\r')) {
				newLine = index + 1;
				break;
			}
		}

		if(newLine > sizeof(cmdBuff)) {
			newLine = sizeof(cmdBuff) - 1;
		}

		if(newLine) {
			uint8_t *pBuf = (uint8_t *)cmdBuff;
			while(newLine--){
				*pBuf++ = fifoPop(&usbRxFifo);
			}

			// If it's an \r\n combination, discard the second one
			if((fifoPeek(&usbRxFifo, 0) == '\n') || (fifoPeek(&usbRxFifo, 0) == '\r')) {
				fifoPop(&usbRxFifo);
			}

			*(pBuf - 1) = 0; // String terminator

			argc = 0;

			// Get command
			argv[argc] = strtok(cmdBuff, " ");

			// Get arguments (if any)
			while ((argv[argc] != NULL) && (argc < sizeof(argv)/sizeof(char *))){
				argc++;
				argv[argc] = strtok(NULL, " ");
			}

			if(argc > 0) {
				command_t *command = commands;
				while(command->commandStr != NULL) {
					if(strcmp(command->commandStr, argv[0]) == 0) {
						command->fn(argc, argv);
						break;
					}
					command++;
				}

				if(command->commandStr == NULL) {
					printf("Unknown command '%s'\n", argv[0]);
					helpFn(1, NULL);
				}
			}
		}
	}
}
