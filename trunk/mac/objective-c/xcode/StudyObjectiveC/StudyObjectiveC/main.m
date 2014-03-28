//
//  main.m
//  StudyObjectiveC
//
//  Created by Shi, Zexing on 1/13/14.
//  Copyright (c) 2014 zexing. All rights reserved.
//

#import <Foundation/Foundation.h>
//#import "SClass.h"
#import "SClass+SCategory.h"
#import "SBlock.h"

int main(int argc, const char * argv[])
{

    @autoreleasepool {
        
        // insert code here...
        NSLog(@"Hello, World!");
        
        SClass * c = [SClass classWithName:@"SimpleClass" AndAge:32];
        NSLog(@"SimpleClass : %@\n", c);
        [c func];
        [c funcWithParam:1];
        [c funcWithParam:2 AndParam2:YES];
        [SClass cfunc];
        [c s_showName];       // call category added method
        NSLog(@"SimpleClass : %@\n", c);
        
        // 2. Test Block
        // 2.1 simple block
        simpleblock();
        // 2.2 complex block
        int a = 2, b = 3;
        int r = addblock(a, b);
        NSLog(@"%d + %d = %d", a, b, r);
        // 2.3 test parse block as callback function
        SBlock * block = [[SBlock alloc] init];
        [block callWithAddBlock:a ParamB:b Block: ^(int i, int j) {
            NSLog(@"%d * %d = %d", i, j, i * j);
        }];
    }
    return 0;
}

