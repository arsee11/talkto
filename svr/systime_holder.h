//systime_holder.h

#ifndef SYSTIME_HOLDER_H
#define SYSTIME_HOLDER_H

#include <time.h>
#include <sys/time.h>

#ifndef STRINGEX_H
#include "stringex.h"
#endif

class SystimeHolder
{
public:
	static size_t Tick()
	{
		struct timespec t;
		clock_gettime(CLOCK_REALTIME, &t);
		return (size_t)t.tv_sec;
	}

	//@return yyyy-mm-dd hh:mm:ss
	static string Now()
	{
		time_t t = time(NULL);
		tm *tm_= localtime(&t);

		string nowstr;
		nowstr+=arsee::t2str(tm_->tm_year+1900)+"-";
		nowstr+=arsee::t2str(tm_->tm_mon+1)+"-";
		nowstr+=arsee::t2str(tm_->tm_mday)+" ";
		nowstr+=arsee::t2str(tm_->tm_hour)+":";
		nowstr+=arsee::t2str(tm_->tm_min)+":";
		nowstr+=arsee::t2str(tm_->tm_sec);

		return nowstr;
	}
};

#endif /*SYSTIME_HOLDER_H*/
