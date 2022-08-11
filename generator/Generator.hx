package ;



#if eval
class Generator {

	// Put any necessary includes in this string and they will be added to the generated files
	static var INCLUDE = "
#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif


#define uchar uchar16
#include <hl.h>
#undef uchar
#include \"hl-opencv.h\"
";
	
	public static function generateCpp() {	
		var options = { idlFile : "lib/opencv/opencv.idl", nativeLib : "opencv", outputDir : "src", includeCode : INCLUDE, autoGC : true };
		webidl.Generate.generateCpp(options);
	}

}
#end