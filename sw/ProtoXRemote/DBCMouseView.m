//
//  DBCMouseView.m
//  DefconBotsControllerTest
//
//  Created by Alvaro Prieto on 8/2/14.
//  Copyright (c) 2014 Alvaro Prieto. All rights reserved.
//

#import "DBCMouseView.h"

#define MAX_POS	(100)

#define THROTTLE_MAX (255)
#define THROTTLE_MIN (0)

#define THROTTLE_SENSITIVITY (16)
#define YAW_SENSITIVITY (10)


NSString * const kPosUpdate = @"kPosUpdate";
NSString * const kPosX = @"kPosX";
NSString * const kPosY = @"kPosY";
NSString * const kPosThrottle = @"kPosThrottle";
NSString * const kPosYaw = @"kPosYaw";

enum {
	upKeyPressed    = 1,
	leftKeyPressed  = 2,
	downKeyPressed  = 4,
	rightKeyPressed = 8
};

@implementation DBCMouseView {
	uint32_t pressedKeys_;
	int32_t throttlePos;
	int32_t yawPos;
}

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		throttlePos = 0;
		yawPos = 0;
    }
    return self;
}

- (void) drawRect:(NSRect)rect {
	
}

- (void)mouseDown:(NSEvent *)theEvent {
    NSPoint mousePos = [theEvent locationInWindow];
    NSRect frame = [super frame];
    int16_t xPos = ((mousePos.x - frame.origin.x) / frame.size.width - 0.5) * 2 * MAX_POS;
    int16_t yPos = ((mousePos.y - frame.origin.y) / frame.size.height - 0.5) * 2 * MAX_POS;
	
	if(xPos > MAX_POS) {
		xPos = MAX_POS;
	}
	
	if(xPos < -MAX_POS) {
		xPos = -MAX_POS;
	}
	
	if(yPos > MAX_POS) {
		yPos = MAX_POS;
	}
	
	if(yPos < -MAX_POS) {
		yPos = -MAX_POS;
	}
	
	NSLog(@"MouseDown %d %d", xPos, yPos);
	[[NSNotificationCenter defaultCenter] postNotificationName:kPosUpdate object:self userInfo:@{kPosX:	[NSNumber numberWithInt:xPos], kPosY:	[NSNumber numberWithInt:yPos]}];

}

- (void)mouseUp:(NSEvent *)theEvent {

}

- (void)mouseDragged:(NSEvent *)theEvent {

	NSPoint mousePos = [theEvent locationInWindow];
    NSRect frame = [super frame];
    int16_t xPos = ((mousePos.x - frame.origin.x) / frame.size.width - 0.5) * 2 * MAX_POS;
    int16_t yPos = ((mousePos.y - frame.origin.y) / frame.size.height - 0.5) * 2 * MAX_POS;
	
	if(xPos > MAX_POS) {
		xPos = MAX_POS;
	}
	
	if(xPos < -MAX_POS) {
		xPos = -MAX_POS;
	}
	
	if(yPos > MAX_POS) {
		yPos = MAX_POS;
	}
	
	if(yPos < -MAX_POS) {
		yPos = -MAX_POS;
	}
	
	[[NSNotificationCenter defaultCenter] postNotificationName:kPosUpdate object:self userInfo:@{kPosX:	[NSNumber numberWithInt:xPos], kPosY:	[NSNumber numberWithInt:yPos]}];
	
	NSLog(@"MouseDragged %d %d", xPos, yPos);
	
}

- (BOOL)acceptsFirstResponder
{
	// YES, we want to be first responder
	return YES;
}

- (void)printPressedKeys
{
	if (pressedKeys_) {
		if (pressedKeys_ & upKeyPressed) {
			throttlePos += THROTTLE_SENSITIVITY;
		}
		if (pressedKeys_ & downKeyPressed) {
			throttlePos -= THROTTLE_SENSITIVITY;
		}
		if (pressedKeys_ & leftKeyPressed) {
			yawPos += YAW_SENSITIVITY;
		}
		if (pressedKeys_ & rightKeyPressed) {
			yawPos -= YAW_SENSITIVITY;
		}
		
		if(throttlePos > THROTTLE_MAX) {
			throttlePos = THROTTLE_MAX;
		} else if(throttlePos < THROTTLE_MIN) {
			throttlePos = THROTTLE_MIN;
		}
		
		if(yawPos > MAX_POS) {
			yawPos = MAX_POS;
		} else if(yawPos < -MAX_POS) {
			yawPos = -MAX_POS;
		}
		
		[[NSNotificationCenter defaultCenter] postNotificationName:kPosUpdate object:self userInfo:@{kPosThrottle:	[NSNumber numberWithInt:throttlePos], kPosYaw:	[NSNumber numberWithInt:yawPos]}];
	}
}

- (void)keyUp:(NSEvent *)anEvent;
{
	BOOL printPressedKeys = YES;
	switch ([anEvent keyCode]) {
		case 0: // 'a'
			pressedKeys_ &= ~leftKeyPressed;
			break;
			
		case 1: // 's'
			pressedKeys_ &= ~downKeyPressed;
			break;
			
		case 2: // 'd'
			pressedKeys_ &= ~rightKeyPressed;
			break;
			
		case 13: // 'w'
			pressedKeys_ &= ~upKeyPressed;
			break;
			
		default:
			// Ignore this key
			printPressedKeys = NO;
	}
	if (printPressedKeys) {
		[self printPressedKeys];
	}
}

- (void)keyDown:(NSEvent *)anEvent;
{
	BOOL printPressedKeys = YES;
	switch ([anEvent keyCode]) {
		case 0: // 'a'
			pressedKeys_ |= leftKeyPressed;
			break;
			
		case 1: // 's'
			pressedKeys_ |= downKeyPressed;
			break;
			
		case 2: // 'd'
			pressedKeys_ |= rightKeyPressed;
			break;
			
		case 13: // 'w'
			pressedKeys_ |= upKeyPressed;
			break;
			
		case 53: // ESC
			// QUIT
			[[NSApplication sharedApplication] terminate:self];
			// Fallthrough
			
		default:
			printPressedKeys = NO;
	}
	
	if (printPressedKeys) {
		[self printPressedKeys];
	}
}

@end
