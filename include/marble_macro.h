
#ifndef MARBLE_MACRO_H
#define MARBLE_MACRO_H

#define _MCAT2(x, y)    x ## y
#define MCAT2(x, y)     _MCAT2(x, y)
#define MCAT3(x, y, z)  MCAT2(x, MCAT2(y, z))

#define MJOIN2(x, y)    MCAT3(x, _, y)
#define MJOIN3(x, y, z) MJOIN2(x, MJOIN2(y, z))

#endif

