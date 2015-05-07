//pack_sender.cpp
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2015-5-7
//record 
//****************************

#include <string>
#include <exception>


#ifndef JPACK_H
#include "mvc/jpack.h"
#endif

#ifndef TCP_SOCK_H 
#include "net/tcpsock.h"
#endif

#include <fstream>

#if defined(_MSC_VER)
#pragma comment(lib, "libmynetd")
//#pragma comment(lib, "mvclibd")
#pragma comment(lib, "jsonlibd")
#endif

typedef arsee::Jpack::pack_ptr_t pack_ptr_t;
typedef arsee::Jpack pack_t;

int GetBuf(char* buf, int blen, const char* filename);

template<class sock>
int SendBuf(sock& s, char *rbuf, size_t rlen)
{
	if (s == nullptr)
		throw exception("not open!");

	typename pack_t::unserial_t us(2048+4+4);
	pack_t pck;
	us(pck, rbuf, rlen);
	if (pck.status())
	{
		s->Write(rbuf, rlen);
	}
	else
	{
		return 0;
	}

	return 1;
}


int main(int argc, char** argv)
{
	arsee::net::SockInit();

	int len=0;
	char buf[2048+4+4] = { 0 };
	if( ( len=GetBuf(buf, 65535, argv[1]) )<= 0 )
	{
		cout <<"get buf error"<<endl;
		return 1;
	}
	
	arsee::net::TcpSock::rpeer_ptr_t tcps = arsee::net::TcpSock::CreateClient("192.168.10.100", 11111);
	if( SendBuf( tcps, buf, len) <= 0 )
	{
		cout <<"send buf error"<<endl;
		return 1;
	}

	arsee::net::SockUninit();
	return 0;
}

int GetBuf(char* buf, int blen, const char* filename)
{
	fstream f;
	f.open(filename, ios::in);

	if( !f.good() )
		return 0;
	
	char buf_tmp[2048] = { 0 };
	char line[1024]={0};
	while( f.getline(line, 1024) )
	{
		strcat_s(buf_tmp, line);
	}

	size_t len = strlen(buf_tmp);
	
	memset(buf, 0xff, 4);
	memcpy(buf+4, &len, 4);
	memcpy(buf + 4+4, buf_tmp, 2048);
	return len+4+4;
}


	

