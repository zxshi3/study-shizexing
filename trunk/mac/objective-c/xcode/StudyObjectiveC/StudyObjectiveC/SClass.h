//
//  SClass.h
//  StudyObjectiveC
//
//  Created by Shi, Zexing on 1/13/14.
//  Copyright (c) 2014 zexing. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * sample class for showing
 * 1. how to define a class
 * 2. how to inherit from super class
 * 3. declare member variable for this class
 * 4. declare member variable with specific attribute
 */
@interface SClass : NSObject    // 1. @interface, class declare keyword
                                // 2. inherit from NSObject

@property NSString* name;       // 3. declare public member variable (property)
@property (readonly) int age;   // 4. add property attributes
@property (weak) id wp;         // 10. declare property as weak reference
@property (copy) NSString * own;    // 11. copy other than strong reference
                                // f you need to set a copy property’s instance variable directly, for example in an initializer method, don’t forget to set a copy of the original object:
                                // __own = [aString copy];

- (instancetype) initWithName:(NSString*)name AndAge:(int)age;  // 10. declare constructor

- (void)func;                   // 5. declare function without parameter and return type
- (void)funcWithParam:(int)v;   // 6. declare function with one parameter
- (NSString *)funcWithParam:(int)v AndParam2:(BOOL)k;   // 7. declare function with multiple parameters

+ (void)cfunc;                  // 8. declare function for this class
+ (instancetype) classWithName:(NSString*)name AndAge:(int)age;  // 9. declare factory method to construct SClass

@end                            // end class declaration
