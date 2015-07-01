#include <cryptopp/files.h>
#include <cryptopp/base64.h>
#include <string>
#include <iostream>

using namespace std;
using namespace CryptoPP;
 
 class MsgCoder
 {
public:
	string E(const char* buf)
	{
		string str;
		StringSource(buf, true, new Base64Encoder( new StringSink(str) ) );
		return std::move(str);
	}

	string D(const char* buf)
	{
		string str;
		StringSource(buf, true, new Base64Decoder( new StringSink(str) ) );
		return std::move(str);
	}
 };



extern string ResolveMsg(const char* msg_type, const char* buf);


template<int msg_type>
class MsgHandler;


template<>
class MsgHandler<MsgTxt>
{
public:
	string FromNet(const char* buf){ return std::move( string(buf) ); }
	string ToNet  (const char* buf){ return std::move( string(buf) ); }
};



template<>
class MsgHandler<MsgJpg>
{
public:
	template<class MsgCoder>
	string FromNet(const char* buf){ return std::move( MsgCoder().D(buf) ); }
	string ToNet  (const char* buf){ return std::move( MsgCoder().E(buf) ); }
};

