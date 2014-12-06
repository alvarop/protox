///////////////////////////////////////////////////////////////////////////////
//
// Simple Fifo implementation
//
// by Alvaro Prieto
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __FIFO__
#define __FIFO__

#include <stdint.h>

typedef struct {
	uint32_t sizeMask;
	uint8_t *buff;
	uint32_t start;
	uint32_t end;
} fifo_t;

int32_t fifoInit(fifo_t *fifo, uint32_t size, void *buff);
uint32_t fifoSize(fifo_t *fifo);
uint8_t fifoPush(fifo_t *fifo, uint8_t byte);
uint8_t fifoPop(fifo_t *fifo);
uint8_t fifoPeek(fifo_t *fifo, uint32_t byte);

#endif
