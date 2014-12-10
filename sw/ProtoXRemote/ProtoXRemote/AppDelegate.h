//
//  AppDelegate.h
//  ProtoXRemote
//
//  Created by Alvaro Prieto on 12/8/14.
//  Copyright (c) 2014 Alvaro Prieto. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>
- (IBAction)StartButton:(id)sender;
@property (weak) IBOutlet NSSlider *ThrottleSlider;
@property (weak) IBOutlet NSSlider *PitchSlider;
@property (weak) IBOutlet NSSlider *YawSlider;
@property (weak) IBOutlet NSSlider *RollSlider;

@end

