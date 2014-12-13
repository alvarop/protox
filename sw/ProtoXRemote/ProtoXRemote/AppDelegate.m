//
//  AppDelegate.m
//  ProtoXRemote
//
//  Created by Alvaro Prieto on 12/8/14.
//  Copyright (c) 2014 Alvaro Prieto. All rights reserved.
//

#import "AppDelegate.h"
#import "DBCConnection.h"
#import "DBCMouseView.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate {
	DBCConnection *connection;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	connection = [[DBCConnection alloc] init];
	
	if([connection openFile:@"/dev/cu.usbmodem1a1361"] < 0) {
		NSLog(@"Error opening serial connection!");
	} else {
		[connection sendCommand:@"help\n"];
	}
	
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(mouseUpdate:)
												 name:kPosUpdate
											   object:nil];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
	// Insert code here to tear down your application
}

- (void) mouseUpdate:(NSNotification *) notification
{
	
	if([[notification userInfo] objectForKey:kPosX]) {
		[connection sendCommand:[NSString stringWithFormat:@"remote r %d\n", (128 - [[[notification userInfo] objectForKey:kPosX] intValue]) ]];
		[_RollSlider setFloatValue:[[[notification userInfo] objectForKey:kPosX] floatValue]];
	}
	if([[notification userInfo] objectForKey:kPosY]) {
		[connection sendCommand:[NSString stringWithFormat:@"remote p %d\n", (128 - [[[notification userInfo] objectForKey:kPosY] intValue]) ]];
		[_PitchSlider setFloatValue:[[[notification userInfo] objectForKey:kPosY] floatValue]];
	}
	
	if([[notification userInfo] objectForKey:kPosYaw]) {
		[connection sendCommand:[NSString stringWithFormat:@"remote y %d\n", (128 - [[[notification userInfo] objectForKey:kPosYaw] intValue]) ]];
		[_YawSlider setFloatValue:[[[notification userInfo] objectForKey:kPosYaw] floatValue]];
	}
	if([[notification userInfo] objectForKey:kPosThrottle]) {
		[connection sendCommand:[NSString stringWithFormat:@"remote t %d\n", ([[[notification userInfo] objectForKey:kPosThrottle] intValue]) ]];
		[_ThrottleSlider setFloatValue:[[[notification userInfo] objectForKey:kPosThrottle] floatValue ]];
		
	}
	
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender { return TRUE; }

- (IBAction)StartButton:(id)sender {
	[connection sendCommand:@"\n\ninit\nremote start\n"];
}
@end
