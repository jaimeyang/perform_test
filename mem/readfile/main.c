#define _GNU_SOURCE
#define _USE_GNU 1
#define BUF_SIZE 32 * 1024 * 1024

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <string.h>
#include <sys/file.h>
#include <fcntl.h>
#include <ctype.h>

long int get_value(char *str)
{
    char *endptr = NULL;
    long int value = strtol(str, &endptr, 10);
    if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN)) || (errno != 0 && value == 0))
    {
        perror("strtol");
        return -1;
    }

    if (endptr == str)
    {
        perror("not number");
        return -1;
    }

    if (value <= 0)
    {
        perror("not positive number");
        return -1;
    }

    return value;
}

void app(const char *disk, long int buffer_size)
{
    int flags = O_RDONLY | O_DIRECT;
    int fd = open(disk, flags, 0755);
    if (fd < 0)
    {
        perror("failed to open disk");
        _exit(1);
    }

    unsigned char *buf;
    struct timeval start, end;
    posix_memalign((void **)&buf,512, buffer_size);
    int read_bytes = 0;
    gettimeofday(&start, NULL);
    while (read_bytes < buffer_size)
    {
        int ret = read(fd, buf, buffer_size);
        if (ret < 0)
        {
            perror("failed to read contents");
            close(fd);
            free(buf);
            _exit(1);
        }
        read_bytes += ret;
    }

    gettimeofday(&end, NULL);
    double total_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Time used: %f s to read %ld bytes\n", total_time, buffer_size);
    close(fd);
    free(buf);
}

int main(int argc, char **argv)
{
    int status = 0;
    int c = 0;
    char *path = NULL;
    char *size = NULL;

    while ((c = getopt(argc, argv, "d:s:")) != -1)
    {
        switch (c)
        {
            case 'd':
                path = optarg;
                break;
            case 's':
                size = optarg;
                break;
            case '?':
                printf("Illegal option: -%c\n", isprint(optopt) ? optopt : '#');
                break;
            default:
                _exit(1);
        }
    }

    long int buffer_size = BUF_SIZE;
    if (size != NULL)
    {
        buffer_size = get_value(size);
        if (buffer_size < 0)
        {
            exit(1);
        }
    }
    for (;;)
    {
        app(path, buffer_size);
        sleep(1);
    }

    return 0;
}