#include "assembler.h"

void		ft_to_token(t_data *d, int type, char *cont, short op)
{
	t_token		*new;
	t_token		*tmp;

	tmp = d->token;
	if (!(new = ft_memalloc(sizeof(t_token))))
		error_malloc(d);
	new->type = type;
	if (!(new->content = ft_strdup(cont)))
	{
		free(new);
		error_malloc(d);
	}
	new->n_l = d->line.n_l;
	new->n_c = d->line.n_c;
	new->n_op = op;
	if (!d->token)
		d->token = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int		is_cmd(t_data *d, char c)
{
	if (c != '\t' && c != ' ' && c != '\"' && c != '\n')
		return (0);
	if (!ft_strcmp(d->line.str, NAME_CMD_STRING) || !ft_strcmp(d->line.str, COMMENT_CMD_STRING))
		ft_to_token(d, COMM, d->line.str, -1);
	else
		return (0);
	d->line.n_c += ft_strlen(d->line.str);
	ft_strclr(d->line.str);
	d->line.i = 0;
	return (1);
}

int		is_str(t_data *d)
{
	char	c;
	int	ret;
	int	n_c;

	ret = 1;
	n_c = d->line.n_c + 1;
	if (d->line.str[0])
		return (-1);
	while (read(d->fd_assm, &c, 1) > 0 && c != '\"')
	{
		d->line.str[d->line.i++] = c;
		c == '\n' ? d->line.n_l += 1 : 0;
		n_c = c == '\n' ? 0 : n_c + 1;
	}
	if (c != '\"')
		ret = -1;
	ft_to_token(d, STRING, d->line.str, -1);
	ft_strclr(d->line.str);
	d->line.i = 0;
	d->line.n_c = n_c + 1;
	return (ret);
}

int		is_end(t_data *d)
{
	char	c;
	int	ret;

	if (d->line.str[0])
		return (-1);
	ft_to_token(d, END, "end", -1);
	d->line.n_l++;
	while ((ret = read(d->fd_assm, &c, 1) > 0) && c == '\n')
		d->line.n_l++;
	if (ret > 0 && c != '\n')
		lseek(d->fd_assm, -1, SEEK_CUR);
	d->line.n_c = 1;
	return (1);
}

int		is_spc(t_data *d)
{
	char	c;
	int	ret;

	if (d->line.str[0])
		return (-1);
	d->line.n_c++;
	while ((ret = read(d->fd_assm, &c, 1) > 0) && (c == ' ' || c == '\t'))
		d->line.n_c++;
	if (ret > 0 && c != ' ' && c != '\t')
		lseek(d->fd_assm, -1, SEEK_CUR);
	return (1);
}

int		is_label(t_data *d)
{
	if (d->line.str[0] == '%' || !d->line.str[0])
		return (0);
	if (!(ft_strchrstr(LABEL_CHARS, d->line.str)))
		return (-1);
	ft_to_token(d, LABEL, d->line.str, -1);
	d->line.n_c += ft_strlen(d->line.str) + 1;
	ft_strclr(d->line.str);
	d->line.i = 0;
	return (1);
}

int		is_instruction(t_data *d)
{
	int	i;
	int	cmp;

	i = 0;
	cmp = 1;
	while (d->op[i].name)
		if (!(cmp = ft_strcmp(d->op[i++].name, d->line.str)))
			break ;
	if (!cmp)
	{
		ft_to_token(d, OPERATION, d->line.str, i - 1);
		d->line.n_c += ft_strlen(d->line.str);
		ft_strclr(d->line.str);
		return (1);
	}
	return (0);
}

int		is_separator(t_data *d)
{
	if (d->line.str[0])
		ft_error_lixic(d);
	ft_to_token(d, SEPARATOR, ",", -1);
	d->line.n_c++;
	return (1);
}

int		is_comment(t_data *d)
{
	char	c;


	while (read(d->fd_assm, &c, 1) > 0)
	{
		if (c == '\n')
		{
			lseek(d->fd_assm, -1, SEEK_CUR);
			return (1);
		}
		d->line.n_c++;
	}
	return (1);
}

int		digit_dir(char *str)
{
	int	i;
	int	nbr;

	nbr = 0;
	i = str[0] == '-' ? 1: 0;
	while (str[i] == '0')
		i++;
	while (ft_isdigit(str[i + nbr]))
		nbr++;///len nuber of dir
	if (str[i + nbr])
		return (0);
	return (1);
}

int		is_direct(t_data *d)
{
	if (d->line.str[1] == LABEL_CHAR)
	{
		if (!(ft_strchrstr(LABEL_CHARS, &d->line.str[2])))
			return (-1);
		ft_to_token(d, _DIR_L, &d->line.str[2], -1);
	}
	else if (!digit_dir(&d->line.str[1]))
		return (-1);
	else
		ft_to_token(d, _DIR, &d->line.str[1], -1);
	d->line.n_c += ft_strlen(d->line.str);
	ft_strclr(d->line.str);
	d->line.i = 0;
	return (1);
}

int		is_registr(t_data *d)
{
	if (!ft_all_digit(&d->line.str[1]))
		return (0);
	if (ft_atoi(&d->line.str[1]) > REG_NUMBER)
		return (-1);
	ft_to_token(d, _REG, &d->line.str[1], -1);
	d->line.n_c += ft_strlen(d->line.str);
	ft_strclr(d->line.str);
	return (1);
}

int		is_indirect(t_data *d)
{
	if (d->line.str[0] == LABEL_CHAR)
	{
		if (!ft_strchrstr(LABEL_CHARS, &d->line.str[1]))
			return (-1);
		ft_to_token(d, _IND_L, &d->line.str[1], -1);
	}
	else if (!digit_dir(d->line.str))
		return (-1);
	else
		ft_to_token(d, _IND, d->line.str, -1);
	d->line.n_c += ft_strlen(d->line.str);
	ft_strclr(d->line.str);
	d->line.i = 0;
	return (1);
}

int		is_arguments(t_data *d)
{
	int	ret;

	ret = 0;
	if (d->line.str[0] == DIRECT_CHAR)
		ret = is_direct(d);
	else if (d->line.str[0] == 'r')
		ret = is_registr(d);
	else if (d->line.str[0] == LABEL_CHAR || ft_isdigit(d->line.str[0]) || d->line.str[0] == '-')
		ret = is_indirect(d);
	return (ret);
}

int		check_token(t_data *d, char c)
{
	int	ret;

	ret = is_cmd(d, c);
	if (ret == 0 && (c == ' ' || c == '\t' || c == '\n' || c == '%' || c == ';' || !c))
		ret = is_instruction(d);
	if (ret == 0 &&  (c == LABEL_CHAR))
		ret = is_label(d);
	if (!ret && (c == SEPARATOR_CHAR || c == '\n' || c == ' ' || c == '\t' || !c))
		ret = is_arguments(d);
	if (ret >= 0 && c == SEPARATOR_CHAR)
		ret = is_separator(d);
	if (ret >= 0 && (c == '\t' || c == ' '))
		ret = is_spc(d);
	if (ret >= 0 && (c == '\"'))
		ret = is_str(d);
	if (ret >= 0 && (c == '\n'))
		ret = is_end(d);
	if (c == COMMENT_CHAR || c == ';')
		ret = is_comment(d);
	if (ret == -1)
		ft_error_lixic(d);
	ret == 1 && c == '%' ? lseek(d->fd_assm, -1, SEEK_CUR) : 0;
	return (ret);
}

void		print_d(t_data *d)//test
{
	t_token *tmp = d->token;
	while (tmp)
	{
		ft_printf("token name [%s] n_op[%d] possition [%03d %03d]\n", tmp->content, tmp->n_op, tmp->n_l, tmp->n_c);
		tmp = tmp->next;
	}
}

void		read_file(t_data *d)
{
	char	c;
	int	loop;

	loop = 1;
	ft_bzero(&d->line, sizeof(d->line));
	if (!(d->line.str = ft_memalloc(MEM_SIZE)))
		error_malloc(d);
	d->line.n_l = 1;
	d->line.n_c = 1;
	while (loop > 0)
	{
		if (!(loop = read(d->fd_assm, &c, 1)))
			c = 0;
		if (check_token(d, c))
			d->line.i = 0;
		else
			d->line.str[d->line.i++] = c;
	}
	ft_strdel(&d->line.str);
	//print_d(d);// test
}
