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
	[connection sendCommand:[NSString stringWithFormat:@"remote r %d\n", (128 - [[[notification userInfo] objectForKey:kPosX] intValue]) ]];
	[connection sendCommand:[NSString stringWithFormat:@"remote p %d\n", (128 - [[[notification userInfo] objectForKey:kPosY] intValue]) ]];
	
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender { return TRUE; }


- (IBAction)throttleChanged:(id)sender {
	int16_t value = (int16_t)([(NSSlider *)sender floatValue]);
	[connection sendCommand:[NSString stringWithFormat:@"remote t %d\n", value]];
}

- (IBAction)yawChanged:(id)sender {
	int16_t value = (int16_t)([(NSSlider *)sender floatValue] + 128);
	[connection sendCommand:[NSString stringWithFormat:@"remote y %d\n", value]];
}

- (IBAction)StartButton:(id)sender {
	[connection sendCommand:@"asdf\n\ninit\n"];
}
@end
