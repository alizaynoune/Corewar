/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhossan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 16:32:21 by akhossan          #+#    #+#             */
/*   Updated: 2020/03/09 14:49:54 by akhossan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "assembler.h"

int		main(int ac, char *av[])
{
	t_data	dat;	

	dat.fd_assm = open(av[1], O_RDONLY);
	if (dat.fd_assm == -1)
	{
		perror(errno);
		exit(1);
	}
	check_commands(&dat);// Checks the syntax of the file commands
	check_code_syntax(&dat);// Syntactic analysis of champion code
	dump_bytecode(&dat);// wirte out bytecode to a .cor file
	clean_up(&data);// close all streams and free all allocated memory (if some)
	return (0);
}

int		check_commands(t_data *dat)
{
		
}
