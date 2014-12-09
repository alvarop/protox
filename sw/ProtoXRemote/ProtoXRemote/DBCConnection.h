//
//  DBCConnection.h
//  DefconBotsControllerTest
//
//  Created by Alvaro Prieto on 2/2/14.
//  Copyright (c) 2014 Alvaro Prieto. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface DBCConnection : NSObject <NSStreamDelegate>
- (int32_t)openFile: (NSString *)filename;
- (void) sendCommand: (NSString *)command;
@end
