#include "memory.h"

void *memset(void *ptr,int c,int size)
{
    char *cptr = (char*)ptr;
    for(int i = 0;i < size;i++)
    {
        cptr[i] = (char) c;
    }
    return ptr;
}

int memcmp(void *c1,void *c2,int count)
{
    char *s1 = c1;
    char *s2 = c2;
    while (count-- > 0)
    {
        if(*s1++ != *s2++)
        {
            return (s1[-1] < s2[-1]) ? -1 : 1;
        }
    }
    return 0;
}