#include <stdio.h>

STYPE bit_reverse(STYPE value) {
    unsigned int bit_stype = 0;
    for (UTYPE mask = 1; mask; mask <<= 1) {
        ++bit_stype;
    }
    UTYPE answ = 0;
    UTYPE uvalue = (UTYPE) value;
    for (int i = 0; i < bit_stype; ++i)
    {
        answ <<= 1;
        answ |= uvalue & 1;
        uvalue >>= 1;
    }
    return (STYPE) answ;
}
