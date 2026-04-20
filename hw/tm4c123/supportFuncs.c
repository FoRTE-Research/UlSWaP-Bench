#include <stddef.h>
#include <stdint.h>

void _exit(int status)
{
    (void)status;
    while(1);
}

/**** Following functions should be replaced with UART implementation. 	****
 **** At the moment they are essentially no-ops.						****/

int _write(int fd, const void *buf, size_t count)
{
    (void)fd;
    (void)buf;
    
	return (int)count;
}

int _read(int fd, void *buf, size_t count)
{
    (void)fd;
    (void)buf;
	(void)count;
    
	return 0;
}
