#include "digma_hw.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

// Low level call for screen update
void epaperUpdate(int ioctl_call)
{
    int fd = open("/dev/fb0", O_RDWR);
    if (fd >= 0)
    {
        ioctl(fd, ioctl_call);
        close(fd);
    }
}
