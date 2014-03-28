//
//  CClass.m
//  StudyObjectiveC
//
//  Created by Shi, Zexing on 1/22/14.
//  Copyright (c) 2014 zexing. All rights reserved.
//

#import "CClass.h"

@implementation CClass

@synthesize count;

- (void)instanceMethod {
    NSLog(@"CClass instanceMethod is called. count = %d", self.count);
}

+ (void)classMethod {
    NSLog(@"CClass classMethod is called");
}

- (NSString *)show:(NSInteger *)i WithMsg:(NSString *)msg {
    self.count = *i;
    NSString * ret = [NSString stringWithFormat:@"i = %ld, msg = %@", (long)*i, msg];
    NSLog(@"%@", ret);
    return ret;
}

@end
