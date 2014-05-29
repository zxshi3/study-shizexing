//
//  REFAppDelegate.m
//  RealEstateFilter
//
//  @see <a href="https://developer.apple.com/library/ios/featuredarticles/ViewControllerPGforiPhoneOS/Introduction/Introduction.html#//apple_ref/doc/uid/TP40007457-CH1-SW1">View Controller Programming Guide</a>
//  @see <a href="https://developer.apple.com/library/ios/documentation/WindowsViews/Conceptual/ViewControllerCatalog/Introduction.html#//apple_ref/doc/uid/TP40011313">View Controller Catalog</a>
//  @see <a href="https://developer.apple.com/library/ios/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/Introduction/Introduction.html#//apple_ref/doc/uid/TP40009503">View Programming Guide</a>
//  @see <a href="https://developer.apple.com/library/ios/documentation/EventHandling/Conceptual/EventHandlingiPhoneOS/Introduction/Introduction.html#//apple_ref/doc/uid/TP40009541">Event Handling Guide</a>
//  @see <a href="http://stackoverflow.com/questions/992348/reading-html-content-from-a-uiwebview">reading html content from a UIWebView</a>
//  Created by Shi, Zexing on 4/4/14.
//  Copyright (c) 2014 SHI.ZEXING. All rights reserved.
//

#import "REFAppDelegate.h"

@implementation REFAppDelegate

// zexings add some textedit & button to the default view
- (void)setupView {
    UIViewController * viewController = self.window.rootViewController;
    if (viewController == nil) {
        NSLog(@"view controller is nil");
    }
    UIView * view = [[self.window rootViewController] view];
    if (view == nil) {
        NSLog(@"view is nil");
    }
    UITextField * tf = [[UITextField alloc] init];
    [view addSubview:tf];
    UIButton * bt = [[UIButton alloc] init];
    [bt setTitle:@"test" forState:UIControlStateNormal];
    [view addSubview:bt];
    UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
//    [button addTarget:self
//               action:@selector(aMethod:)
//     forControlEvents:UIControlEventTouchUpInside];
    [button setTitle:@"Show View" forState:UIControlStateNormal];
    button.frame = CGRectMake(80.0, 210.0, 160.0, 40.0);
    [view addSubview:button];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    // zexings add
    [self setupView];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
