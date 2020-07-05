#include "ft_dprintf.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int	main(int ac, char **av)
{
	int	ft = 0;
	int	sys = 0;
	int	fd;
	char	*str;
	long long int value;
	fd = 1;
	if (ac >= 4 && av[1][0] == 'd')
	{
		str = av[2];
		value = atoll(av[3]);
		dprintf(fd, "[ret %d]\n",ft_dprintf(fd, str, value));
		dprintf(fd, "[ret %d]\n",dprintf(fd, str, value));
	}
	if (ac >= 4 && av[1][0] == 's')
	{
		str = av[2];
		dprintf(fd, "[ret %d]\n",ft_dprintf(fd, str, av[3]));
		dprintf(fd, "[ret %d]\n",dprintf(fd, str, av[3]));
	}
}
