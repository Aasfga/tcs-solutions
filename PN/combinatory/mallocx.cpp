#include <sys/mman.h>

static char * memory = (char *) mmap(0, 4096, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

char * mallocx(unsigned long size)
{
	memory += size;
	return (memory-size);
}
