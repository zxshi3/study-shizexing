//
//  SBlock.h
//  StudyObjectiveC
//
//  Created by Shi, Zexing on 1/27/14.
//  Copyright (c) 2014 zexing. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * example to show Block
 */
void (^simpleblock)(void);

/**
 * block take arguments and return value
 */
int (^addblock)(int, int);
typedef int(^AddBlock)(int, int);

@interface SBlock : NSObject

@property (copy) AddBlock addBlock;         // 1. copy keyword is must to use for Block property
                                            // Ref : https://developer.apple.com/library/ios/documentation/Cocoa/Conceptual/ProgrammingWithObjectiveC/WorkingwithBlocks/WorkingwithBlocks.html

- (void)callWithAddBlock:(int)a ParamB:(int)b Block:(void (^)(int, int))f;

@end