//modelsys.h
//absract: persistent objects about system level config.

#ifndef MODLESYS_H
#define MODLESYS_H

#pragma db object
class Sys
{
	friend class odb::access;
	
public:

private:
	size_t _onlines;
	size_t _max_onlines;
	int    _port;
	
};


#endif /*MODLESYS_H*/