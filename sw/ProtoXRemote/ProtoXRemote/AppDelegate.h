//
//  AppDelegate.h
//  ProtoXRemote
//
//  Created by Alvaro Prieto on 12/8/14.
//  Copyright (c) 2014 Alvaro Prieto. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>
- (IBAction)throttleChanged:(id)sender;
- (IBAction)yawChanged:(id)sender;
- (IBAction)StartButton:(id)sender;

@end

