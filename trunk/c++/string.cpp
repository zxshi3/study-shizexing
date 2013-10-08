/**
 * study std::string functionality
 */
#include <string>
#include <iostream>

namespace shi {
namespace zexing {
class string {
public:
	void constructor();
    void find();
};
}	// zexing
}	// shi

int main(int argc, char** argv) {
	shi::zexing::string str;
	str.constructor();
    str.find();
	return 0;
}

void shi::zexing::string::constructor() {
/*
 * string ( );
 *  Content is initialized to an empty string.
 */
	std::string str1;
	std::cout << "default string() : " << str1 << std::endl;
/*
 * string ( const string& str );
 *  Content is initialized to a copy of the string object str.
 */
/*
 * string ( const string& str, size_t pos, size_t n = npos );
 *  Content is initialized to a copy of a substring of str. The substring is the portion of str that begins at the
 *  character position pos and takes up to n characters (it takes less than n if the end of str is reached before).
 */
/*
 * string ( const char * s, size_t n );
 *  Content is initialized to a copy of the string formed by the first n characters in the array of characters 
 *  pointed by s.
 */
	char two_string[] = {'h', 'e', 'l', 'l', 'o', '\0', 'w', 'o', 'r', 'l', 'd', '\0', '!'};	// sizeof == 13
	//char two_string[] = "hello\0world\0!";	// sizeof == 14
	std::cout << "two_string : " << two_string << std::endl
		<< "sizeof \"hello\\0world\\0!\" is " << sizeof(two_string) << std::endl;
	std::string str4(&two_string[0], sizeof(two_string));
	std::cout << "str4 : " << str4 << std::endl
		<< "str4.c_str() : " << str4.c_str() << std::endl
		<< "str4.length() : " << str4.length() << std::endl
		<< "str4.size() : " << str4.size() << std::endl;
	std::string str4copy(str4);
	str4 = "another string";
	std::cout << "str4copy : " << str4copy << std::endl
		<< "str4copy.c_str() : " << str4copy.c_str() << std::endl
		<< "str4copy.length() : " << str4copy.length() << std::endl
		<< "str4copy.size() : " << str4copy.size() << std::endl;
/*
 * string ( const char * s );
 *  Content is initialized to a copy of the string formed by the null-terminated character sequence (C string) 
 *  pointed by s. The length of the character sequence is determined by the first occurrence of a null character (as 
 *  determined by traits.length(s)). This version can be used to initialize a string object using a string literal 
 *  constant.
 * string ( size_t n, char c );
 *  Content is initialized as a string formed by a repetition of character c, n times.
 * template<class InputIterator> string (InputIterator begin, InputIterator end);
 *  If InputIterator is an integral type, behaves as the sixth constructor version (the one right above this) by 
 *  typecasting begin and end to call it:
 *  string(static_cast<size_t>(begin),static_cast<char>(end)); 
 *  In any other case, the parameters are taken as iterators, and the content is initialized with the values of the 
 *  elements that go from the element referred by iterator begin to the element right before the one referred by 
 *  iterator end.
*/
}

void shi::zexing::string::find() {
    std::string header = "Accept: text/html\r\nAccept-Encoding: gzip, deflate\r\nUser-Agent: zexing\r\nKeep-Alive: 300\r\n\r\n";
    std::vector<std::string> headers;
    std::size_t pos = 0;
    while(pos != std::string::npos) {
        std::size_t fd = header.find("\r\n", pos);
        if (fd == std::string::npos) {
            break;
        }
        headers.push_back(header.substr(pos, fd + sizeof("\r\n")));
        pos = fd + sizeof("\r\n") + 1;
    }
    std::cout << "--- find test ---\n";
    std::cout << "original string = " << header;
    std::cout << "header = \n";
    for (std::vector<std::string>::const_iterator it = headers.begin(); it != headers.end(); it++) {
        std::cout << *it;
    }
}
