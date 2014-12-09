//
//  DBCConnection.m
//  DefconBotsControllerTest
//
//  Created by Alvaro Prieto on 2/2/14.
//  Copyright (c) 2014 Alvaro Prieto. All rights reserved.
//

#import "DBCConnection.h"

@implementation DBCConnection {
	int serialFd;
	NSThread *readThread;
}

static uint8_t buff[1024];

- (void) readThread {
	ssize_t rd;

	NSLog(@"readThread started");
	
	while (1) {
		rd = read(serialFd, buff, sizeof(buff));
		if(rd > 0) {
			for(int32_t x = 0; x < rd; x++) {
				putchar(buff[x]);
			}
		} else {
			break;
		}
	}
	NSLog(@"done");
}

- (id) init {
	self = [super init];
	
	if(self) {
		serialFd = -1;
	}
	
	readThread = [[NSThread alloc] initWithTarget:self
										 selector:@selector(readThread)
										   object:nil];
	
	return self;
}

- (int32_t)openFile: (NSString *)filename {
	serialFd = open([filename cStringUsingEncoding:NSUTF8StringEncoding], O_RDWR);
	
	if(serialFd >= 0) {
		[readThread start];
	}
	
	return serialFd;
}

- (void)sendCommand: (NSString *)command {
	if(serialFd) {
		write(serialFd, [command cStringUsingEncoding:NSUTF8StringEncoding], [command length]);
		NSLog(@"%@",command);
	}
}

- (void)close {
	if(serialFd) {
		close(serialFd);
	}
}

@end
