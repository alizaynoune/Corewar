/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhossan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/07 20:33:03 by akhossan          #+#    #+#             */
/*   Updated: 2020/03/08 15:54:30 by akhossan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int		main(int ac, char **av)
{
	char	typecode;
	char	instr_code;	
	char	p1;
	short	p2;
	short	p3;
	int		fd;

	fd = 1;
	if (*(char *)&fd == 1)
		printf("Little endian\n");
	else
		printf("Big endian\n");
	exit(1);
	typecode = 11;
	instr_code = 104;
	p1 = 1;	
	p2 = 7;
	p3 = 1;
	fd = open(av[1], O_CREAT | O_RDWR | O_APPEND);
	write(fd, &typecode, 1);
	write(fd, &instr_code, 1);
	write(fd, &p1, 1);
	char	y;
	y = p2 >> 8;
	write(fd, &y, 1);
	y = p2 & 0xff;
	write(fd, &y, 1);
	y = (p3 >> 8) & 0xff;
	write(fd, &y, 1);
	y = p3 & 0xff;
	write(fd, &y, 1);
	close(fd);
	return (0);
}
