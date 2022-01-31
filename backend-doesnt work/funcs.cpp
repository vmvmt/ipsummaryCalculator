#include <string>
#include <vector>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace emscripten;
typedef unsigned int IP;

//Assumes you don't have more than 64 networks
val summaryIP(val values)
{
	std::vector<std::string> argv=vecFromJSArray<std::string>(values);
	size_t argc=argv.size();

	IP* converted = new IP[argc];
	IP* masks = new IP[argc];

	for (size_t i{ 0 }; i < argc; ++i)
	{
		int pos{3};
		size_t afterNumber=0;
		IP _result;
		while (pos>-1)
		{
			_result=std::stoi(argv[i],&afterNumber);
			converted[i] += (_result << (pos * 8));
			argv[i]=argv[i].substr(afterNumber+1);
            --pos;
		}
		masks[i]=std::stoi(argv[i],nullptr);
		converted[i]&=~(( 1U << ( 32-(masks[i]) ) )-1);// Mask for subnetting
	}

	size_t resultmask = 0;
	IP mask=1<<31;
	while(mask!=0)
	{
		for(size_t i{1};i<argc;++i)
        {
            if((converted[0]&mask)!=(converted[i]&mask)) break;
        }
        mask >>=1;
		++resultmask;
	}
	for(size_t i{};i<argc;++i){
	if(masks[i]<resultmask) resultmask=masks[i];	
	}

	IP result{};

	switch (resultmask)
	{
	case 0:
		break;
	case 32:
		result=converted[0];
		break;
	default:
		result=converted[0]&(~((1U<<(32-resultmask))-1));
		break;
	} 

	std::string valToReturn;
	mask=0xFF000000;
	int i{3};
	while (mask>0)
	{
		valToReturn+=std::string(std::to_string((result&mask)>>(i*8))+';');
		mask>>=8;
		--i;
	}
	valToReturn+=std::to_string(resultmask);
	return val(valToReturn);
}
EMSCRIPTEN_BINDINGS(module)
{
	function("summaryIP",&summaryIP); 
}