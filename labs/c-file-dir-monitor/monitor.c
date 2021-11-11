#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include "logger.h"

#define MAX_EVENT_MONITOR 2048
#define NAME_LEN 32
#define MONITOR_EVENT_SIZE (sizeof(struct inotify_event))
#define BUFFER_LEN MAX_EVENT_MONITOR *(MONITOR_EVENT_SIZE + NAME_LEN)

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        errorf("ERROR: Too few arguments");
        exit(2);
    }
    else
    {

        const char *path = argv[1];
        int fd, watch_desc;
        char buffer[BUFFER_LEN];
        fd = inotify_init();

        if (fd < 0)
        {
            perror("ERROR: Notify has an error");
        }

        watch_desc = inotify_add_watch(fd, path, IN_CREATE | IN_MOVED_FROM | IN_MOVED_TO | IN_DELETE);

        if (watch_desc == -1)
        {
            warnf("Warning: Couldn't add watch to %s \n", path);
        }
        else
        {
            infof("Starting File/Directory Monitor on %s \n", path);
            printf("\n");
        }

        int i = 0;

        while (1)
        {
            i = 0;
            int total_read = read(fd, buffer, BUFFER_LEN);

            if (total_read < 0)
            {
                perror("Error: read error");
            }

            while (i < total_read)
            {
                struct inotify_event *event = (struct inotify_event *)&buffer[i];

                if (event->len)
                {
                    if (event->mask & IN_CREATE)
                    {
                        if (event->mask & IN_ISDIR)
                        {
                            infof("- [Directory - Create]: %s \n", event->name);
                        }
                        else
                        {
                            infof("- [File Create]: %s \n", event->name);
                        }
                    }
                    else if (event->mask & IN_DELETE)
                    {
                        if (event->mask & IN_ISDIR)
                        {
                            infof("- [Directory - Removal]: %s\n", event->name);
                        }
                        else
                        {
                            infof("- [File removal]: %s\n", event->name);
                        }
                    }
                    else if (event->mask & IN_MOVED_FROM)
                    {
                        if (event->mask & IN_ISDIR)
                        {
                            infof("- [Directory - Rename]: %s", event->name);
                        }
                        else
                        {
                            infof("- [File rename]: %s", event->name);
                        }
                    }
                    else if (event->mask & IN_MOVED_TO)
                    {
                        if (!(event->mask & IN_ISDIR))
                        {
                            infof(" -> %s \n", event->name);
                        }
                    }

                    i += MONITOR_EVENT_SIZE + event->len;
                }
            }
        }

        inotify_rm_watch(fd, watch_desc);
        close(fd);
        return (EXIT_SUCCESS);
    }
}