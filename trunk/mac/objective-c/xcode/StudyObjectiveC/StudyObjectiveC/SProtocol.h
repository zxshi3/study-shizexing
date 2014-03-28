//
//  SProtocol.h
//  StudyObjectiveC
//
//  Created by Shi, Zexing on 1/22/14.
//  Copyright (c) 2014 zexing. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol SProtocol <NSObject>  // 4. inherit from NSObject protocol

+ (void)classMethod;        // 1. protocol can declare class method
- (void)instanceMethod;     // 2. protocol can declare instance method
@property int count;        // 3. protocol can declare property. you should synthesize it in class which conform to this protocol

@optional                   // all method declared under @optional directive is marked as optional. until another directive
-(void)instanceMethodOpt; // 5. declare optional instance method
-(int)instanceMethodKey:(NSString*)k AndValue:(NSString*)v;
// ...

@required

@end

@protocol SProtocol2 <NSObject>

@optional
-(NSString*)show:(NSInteger*)i WithMsg:(NSString*)msg;

@end
