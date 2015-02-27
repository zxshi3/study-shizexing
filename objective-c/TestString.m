#import <Foundation/Foundation.h>

@interface StringTester : NSObject
-(void)testSplit;
@end

@implementation StringTester
-(void)testSplit {
	NSArray * ss = [@"abc\nefd" componentsSeparatedByString:@"\n"];
	for (int i = 0; i < [ss count]; ++i) {
		NSString * s = [ss objectAtIndex:i];
		NSLog(@"%@", s);
	}
}
@end

// how to compile
// gcc -framework Foundation TestString.m -o teststring
int main(int argc, const char * argv[]) {
	NSLog(@"hello objective c");
	StringTester * tester = [[StringTester alloc] init];
	[tester testSplit];
}
