#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

typedef unsigned int IP;
using namespace std;

string summaryIP(vector<string>& argv);

int main(){
    vector<string> input{"192;168;1;1;24", "192;168;0;1;24", "192;168;0;1;24", "192;168;0;1;12"};
    cout << summaryIP(input);
    return 0;
}


//Assumes you don't have more than 64 networks
string summaryIP(vector<string>& argv)
{
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
			_result=stoi(argv[i],&afterNumber);
			converted[i] += (_result << (pos * 8));
			argv[i]=argv[i].substr(afterNumber+1);
			cout << i << ". element " << argv[i] << '\n';
            --pos;
		}
		masks[i]=stoi(argv[i],nullptr);
		converted[i]&=~(( 1U << ( 32-(masks[i]) ) )-1);// Mask for subnetting
	}

	size_t resultmask = 0;
	IP mask=1<<31;
    uint64_t bitmask=(1<<argc)-1;
    uint64_t onebitacc=0;
	while(mask!=0)
	{
		for(uint64_t i{0};i<argc;++i)
        {
            onebitacc+=(((converted[i]&mask)>0)<<i);
        }
        if(((onebitacc&bitmask)-1)<(bitmask-1)) break;
        onebitacc=0;
        mask = mask >> 1;
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

	string valToReturn;
	mask=0xFF000000;
	int i{3};
	while (mask>0)
	{
		valToReturn+=string(to_string((result&mask)>>(i*8))+';');
		mask>>=8;
		--i;
	}
	valToReturn+=std::to_string(resultmask);
	return valToReturn;
}