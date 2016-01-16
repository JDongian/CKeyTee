#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define BUFFER_LENGTH   256
#define KEY_RELEAS      0
#define KEY_PRESS       1
#define KEY_REPEAT      2

int main(int argc, char **argv, char **envp)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <path/to/keyboard-device>\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    struct input_event events[BUFFER_LENGTH];
    while(1)
    {
        ssize_t bytes_read = read(fd, events, sizeof(events));
        for (int i = 0; i < bytes_read / sizeof(struct input_event); i++)
        {
            if (events[i].type == EV_KEY && 
                    (events[i].value == KEY_PRESS || events[i].value == KEY_REPEAT))
            {
                printf("Key Code: %X\n", events[i].code);
            }
        }
    }
}
