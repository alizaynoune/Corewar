/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhossan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 15:11:23 by akhossan          #+#    #+#             */
/*   Updated: 2020/03/10 18:04:31 by akhossan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

int		main(int ac, char **av)
{
	int		fd;
	char	b;

	fd = open(av[1], O_RDONLY);
	while (read(fd, &b, 1) > 0 && b != '\n')
	{
		write(1, &b, 1);
	}
	printf("line: %d\n", __LINE__);
	write(1, "\n", 1);

	printf("ret: %d\n", lseek(fd, 1, SEEK_CUR));
	read(fd, &b, 1);
	write(1, &b, 1);

	write(1, "\n", 1);
	return (0);
}
