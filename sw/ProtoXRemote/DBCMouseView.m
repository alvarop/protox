//
//  DBCMouseView.m
//  DefconBotsControllerTest
//
//  Created by Alvaro Prieto on 8/2/14.
//  Copyright (c) 2014 Alvaro Prieto. All rights reserved.
//

#import "DBCMouseView.h"

#define MAX_POS	(100)

NSString * const kPosUpdate = @"kPosUpdate";
NSString * const kPosX = @"kPosX";
NSString * const kPosY = @"kPosY";

@implementation DBCMouseView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {

    }
    return self;
}

- (void) drawRect:(NSRect)rect {
	
}

- (void)mouseDown:(NSEvent *)theEvent {
    NSPoint mousePos = [theEvent locationInWindow];
    NSRect frame = [super frame];
    int16_t xPos = ((mousePos.x - frame.origin.x) / frame.size.width - 0.5) * MAX_POS;
    int16_t yPos = ((mousePos.y - frame.origin.y) / frame.size.height - 0.5) * MAX_POS;
	
	if(xPos > MAX_POS/2) {
		xPos = MAX_POS/2;
	}
	
	if(xPos < -MAX_POS/2) {
		xPos = -MAX_POS/2;
	}
	
	if(yPos > MAX_POS/2) {
		yPos = MAX_POS/2;
	}
	
	if(yPos < -MAX_POS/2) {
		yPos = -MAX_POS/2;
	}
	
	NSLog(@"MouseDown %d %d", xPos, yPos);
		[[NSNotificationCenter defaultCenter] postNotificationName:kPosUpdate object:self userInfo:@{kPosX:	[NSNumber numberWithInt:xPos], kPosY:	[NSNumber numberWithInt:yPos]}];

}

- (void)mouseUp:(NSEvent *)theEvent {

}

- (void)mouseDragged:(NSEvent *)theEvent {

	NSPoint mousePos = [theEvent locationInWindow];
    NSRect frame = [super frame];
    int16_t xPos = ((mousePos.x - frame.origin.x) / frame.size.width - 0.5) * MAX_POS;
    int16_t yPos = ((mousePos.y - frame.origin.y) / frame.size.height - 0.5) * MAX_POS;
	
	if(xPos > MAX_POS/2) {
		xPos = MAX_POS/2;
	}
	
	if(xPos < -MAX_POS/2) {
		xPos = -MAX_POS/2;
	}
	
	if(yPos > MAX_POS/2) {
		yPos = MAX_POS/2;
	}
	
	if(yPos < -MAX_POS/2) {
		yPos = -MAX_POS/2;
	}
	
	[[NSNotificationCenter defaultCenter] postNotificationName:kPosUpdate object:self userInfo:@{kPosX:	[NSNumber numberWithInt:xPos], kPosY:	[NSNumber numberWithInt:yPos]}];
	
	NSLog(@"MouseDragged %d %d", xPos, yPos);
    
}

@end
