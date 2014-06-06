
#ifndef MECSTRING_H
#define MECSTRING_H

#define ECS gnu

#if ECS == gnu
#include <string>
using namespace std;
#define ECString string
#else
#include <bstring.h>
#define ECString string
#endif

#endif	/* ! MECSTRING_H */
