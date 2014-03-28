//
//  SBlock.m
//  StudyObjectiveC
//
//  Created by Shi, Zexing on 1/27/14.
//  Copyright (c) 2014 zexing. All rights reserved.
//

#import "SBlock.h"

static int s_n = 2014;

//simpleblock = ^ {                 // compile error
void (^simpleblock)(void) = ^ {
    NSLog(@"simple block, static number = %d", s_n);    // variable life cycle is similar to C,
                                                        // but the stack variable value is captured when the block is defined
};

//addblock = ^(int a, int b) {      // compile error
//int (^addblock)(int, int) = ^(int a, int b) {         // ok
//int (^addblock)(int, int) = ^int(int a, int b) {        // ok, more clear
AddBlock addblock = ^int(int a, int b) {                // ok
    int n = 2;
    __block int sharedVar = 10;
    void (^anotherblock)(void) = ^ {
        NSLog(@"n = %d, a = %d, s_n = %d, sharedVar = %d", n, a, s_n, sharedVar);
    };
    
    n = 5;
    sharedVar = 20;
    
    anotherblock();                                     // *** n == 2, sharedVar == 20
    
    simpleblock();                                      // 2014
    s_n = 15;
    simpleblock();                                      // 15
    
    anotherblock();                                     // n == 2, s_n == 15, stack variable is captured when the block is defined
    
    return a + b;
};

@implementation SBlock

- (void)callWithAddBlock:(int)a ParamB:(int)b Block:(void (^)(int, int))f {
    f(a, b);
}

@end