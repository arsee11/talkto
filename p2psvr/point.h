//point.h

#inlcude "arsee/udpsock.h"

class Point
{
public:
	typedef unsigned log id_t;

	id_t id;
	AddrPair real_addr;
	AddrPair nat_addr
};


typedef map<Point::id_t, Point> points_t;

:
