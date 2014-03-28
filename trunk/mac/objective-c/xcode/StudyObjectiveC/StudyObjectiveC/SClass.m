//
//  SClass.m
//  StudyObjectiveC
//
//  Created by Shi, Zexing on 1/13/14.
//  Copyright (c) 2014 zexing. All rights reserved.
//

#import "SClass.h"

@implementation SClass

- (instancetype)initWithName:(NSString *)name AndAge:(int)age {
    // SClass * ret = [[SClass alloc] init];
    self = [super init];
    if (self) {
        self.name = name;
        _age = age;     // 3. assign readonly property. by default xcode use _$property as its name
                        //   if you want to assign other name, use syntax inside @implementaion
                        //   @synthesize propertyName = instanceVariableName;
                        //   @synthesize propertyName;   equals  @synthesize propertyName=propertyName;
                        //
                        //   if you implement both getter and setter for a readwrite property or
                        //   a getter for a readonly property
                        //   you need to add @synthesize property = _property;
    }
    return self;
}

- (void)func {  // 1. implement method
    NSLog(@"func is called\n");
    _age = 10;              // 4. assign readonly property is ok
}

- (void)funcWithParam:(int)v {
    // self.age = v;       // error : assignement to readonly property
    NSLog(@"input parameter = %d\n", v);
}

- (NSString *)funcWithParam:(int)v AndParam2:(BOOL)k {
    NSString * ret = [NSString stringWithFormat:@"v = %d, k is %d", v, k];      // call class method
    NSLog(@"%@\n", ret);
    return ret;             // return value
}

- (NSString *)description {
    // Override description inherit from NSObject
    NSString * __weak wp = self.name;       // 4. declare weak reference
    NSString * ret = [NSString stringWithFormat:@"name = %@, age = %d", self.name, self.age];
    return ret;
}

+ (void)cfunc {
    NSLog(@"class function");
}

+ (instancetype)classWithName:(NSString *)name AndAge:(int)age { // 2. implement factory method
    SClass * ret = [[SClass alloc] initWithName:name AndAge:age];
    return ret;
}

@end
