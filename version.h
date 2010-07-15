#ifndef VERSION_H
#define VERSION_H

namespace DuplicateLinesFinder{
	
	//Date Version Types
	static const char DATE[] = "16";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2010";
	static const char UBUNTU_VERSION_STYLE[] = "10.07";
	
	//Software Status
	static const char STATUS[] = "Release";
	static const char STATUS_SHORT[] = "r";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 0;
	static const long BUILD = 1;
	static const long REVISION = 7;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 10;
	#define RC_FILEVERSION 1,0,1,7
	#define RC_FILEVERSION_STRING "1, 0, 1, 7\0"
	static const char FULLVERSION_STRING[] = "1.0.1.7";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 1;
	

}
#endif //VERSION_H
