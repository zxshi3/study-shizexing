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

@end                            // end class declaration
