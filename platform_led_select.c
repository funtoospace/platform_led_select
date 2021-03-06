#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

/*   platform_led on
  *  platform_led off
  *  platform_led read
  */
int main(int argc, char **argv)
{
	int fd;
	int state;
	int val = 1;
	fd_set rfds;


	fd = open("/dev/platform_led", O_RDWR | O_NONBLOCK);
	if (fd < 0)
	{
		printf("can't open!\n");
	}
	if (argc != 2)
	{
		printf("Usage :\n");
		printf("%s <on|off>\n", argv[0]);
		return 0;
	}


	if (strcmp(argv[1], "on") == 0)
	{
		val  = 1;
		write(fd, &val, 4);
	}
	else if (strcmp(argv[1], "off") == 0)
	{
		val = 0;
		write(fd, &val, 4);
	}
	else if (strcmp(argv[1], "read") == 0)
	{
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

		select(fd + 1, &rfds, NULL, NULL, NULL);

		if (FD_ISSET(fd, &rfds))
			printf("Poll monitor: platform_led is on, it can be read\n");
		else
			printf("Poll monitor: platform_led is off, enter wait-queue\n");
		
		read(fd, &state, 4);
		if (state == 1)
			printf("platform_led is on, state = %d\n",state);
		else
			printf("platform_led is off, state = %d\n", state);
	}
	
	return 0;
}
