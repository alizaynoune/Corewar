/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:47:03 by alzaynou          #+#    #+#             */
/*   Updated: 2020/03/13 19:00:03 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/assembler.h"

void			exit_error(char *msg)
{
	ft_printf("%s\n", msg);
	exit (1);
}

void		ft_error_file(char *file, t_data *data)
{
	char		c;
	int			err;

	if ((data->fd_assm = open(file, O_RDONLY)) < 0)
		exit_error(strerror(errno));
	else if ((err = read(data->fd_assm, &c, 1)) <= 0)
	{
		close(data->fd_assm);
		if (err < 0)
			exit_error(strerror(errno));
		exit_error("Empty file");
	}
	lseek(data->fd_assm, -1, SEEK_CUR);
}

size_t		skip_space(char *str)
{
	size_t	cnt;

	cnt = 0;
	while (str[cnt] == ' ' || str[cnt] == '	')
		cnt++;
	return (cnt);
}

int		match_quotation(char *str)
{
	size_t	cnt;

	cnt = 0;
	while (str[cnt])
	{
		if (str[cnt++] == '"')
			return (1);
	}
	return (0);
}

void		error_name_commen(char *err, size_t lin, size_t pos, char *line)
{
	ft_strdel(&line);
	ft_printf("%s [%03d : %03d]\n", err, lin, pos);
	exit (1);
}

void		get_sting(t_data *data, char *line, size_t curr, int8_t flag)
{
	char			*ptr;
	char			*tmp;
	unsigned int	nbr;

	nbr = data->lines.nb_line;
	curr += skip_space(&line[curr]);
	if (line[curr] != '"')
		error_name_commen("Lexical error at", nbr, ++curr, line);
	line = &line[curr];
	if (!match_quotation(line + 1))
	{
		while (1)
		{
			get_next_line(data->fd_assm, &data->lines.line);
			data->lines.nb_line++;
			line = ft_strjoin(line, data->lines.line);
			if (match_quotation(data->lines.line))
				break;
			ft_strdel(&data->lines.line);
		}
	}
	ft_printf("=>%s", line);
}

void		name_commant(t_data *data)
{
	int		err;
	int		ret;
	size_t	spc;

	err = 0;
	while ((ret = get_next_line(data->fd_assm, &data->lines.line)) > 0)
	{
		data->lines.nb_line++;
		spc = skip_space(data->lines.line);
		if (data->lines.line[spc] && data->lines.line[spc] != '\n' && data->lines.line[spc] != '#')
		{
			if (!ft_strncmp(".name", &data->lines.line[spc], 5))
				err += err == 1 ? err * -1 : 1;
			else if (!ft_strncmp(".comment", &data->lines.line[spc], 8))
				err += err == 2 ?err * -1 : 2;
			else
				error_name_commen("Lexical error at", data->lines.nb_line, spc, data->lines.line);
			err == 0 ? exit_error("error1") : 0;
			spc += err == 1 ? 5 : 8;
			get_sting(data, data->lines.line, spc, err);
			err == 3 ? exit_error("true") : 0;
		}
		ft_strdel(&data->lines.line);
	}
}

int			main(int ac, char **av)
{
	t_data		data;

	ft_bzero(&data, sizeof(t_data));
	ft_bzero(&data.lines, sizeof(t_line));
	if (ac > 1)
	{
		ft_error_file(av[ac - 1], &data);
		name_commant(&data);
	}
	else if (ac <= 1)
		exit_error("Usage: ./asm <sourcefile.s>");
	return (0);
}
