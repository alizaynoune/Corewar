#include "assembler.h"
// stock to token
void		tokenizer(t_all *d, int type, char *cont, short op)
{
	t_token		*new;

	if (!(new = ft_memalloc(sizeof(t_token))))
		error_malloc(d);
	// type == LABEL or STRING or SEPARATOR or arg......
	new->type = type;
	// cont == content lik string or name of label .....
	if (!(new->content = ft_strdup(cont)))
	{
		free(new);
		error_malloc(d);
	}
	// number of line
	new->nbr_l = d->line.nbr_l;
	// possition of charater
	new->pos = d->line.pos;
	// op == possition of operation in op_tab
	new->index_op = op;
	if (!d->token)
	{
		d->token = new;
		d->ptr_token = new;
	}
	else
	{
		d->ptr_token->next = new;
		d->ptr_token = d->ptr_token->next;
	}
}
// check is comment or name
int		is_cmd(t_all *d, char c)
{
	if (c != '\t' && c != ' ' && c != '\"' && c != '\n')
		return (0);
	if (!ft_strcmp(d->line.str, NAME_CMD_STRING) || !ft_strcmp(d->line.str, COMMENT_CMD_STRING))
		tokenizer(d, COMM, d->line.str, -1);
	else
		return (0);
	d->line.pos += ft_strlen(d->line.str);
	ft_strclr(d->line.str);
	d->line.i = 0;
	return (1);
}
// read string
int		is_str(t_all *d)
{
	char	c;
	int	ret;
	int	pos;

	ret = 1;
	pos = d->line.pos + 1;
	if (d->line.str[0])
		return (-1);
	while (read(d->fd_assm, &c, 1) > 0 && c != '\"')
	{
		d->line.str[d->line.i++] = c;
		c == '\n' ? d->line.nbr_l += 1 : 0;
		pos = c == '\n' ? 0 : pos + 1;
	}
	if (c != '\"')
		ret = -1;
	tokenizer(d, STRING, d->line.str, -1);
	ft_strclr(d->line.str);
	d->line.i = 0;
	d->line.pos = pos + 1;
	return (ret);
}
// end of line
int		is_end(t_all *d)
{
	char	c;
	int	ret;

	if (d->line.str[0])
		return (-1);
	tokenizer(d, END, "end", -1);
	d->line.nbr_l++;
	while ((ret = read(d->fd_assm, &c, 1) > 0) && c == '\n')
		d->line.nbr_l++;
	if (ret > 0 && c != '\n')
		lseek(d->fd_assm, -1, SEEK_CUR);
	d->line.pos = 1;
	return (1);
}
// skip space and tap
int		is_spc(t_all *d)
{
	char	c;
	int	ret;

	if (d->line.str[0])
		return (-1);
	d->line.pos++;
	while ((ret = read(d->fd_assm, &c, 1) > 0) && (c == ' ' || c == '\t'))
		d->line.pos++;
	if (ret > 0 && c != ' ' && c != '\t')
		lseek(d->fd_assm, -1, SEEK_CUR);
	return (1);
}
// check is label
int		is_label(t_all *d)
{
	// if index 0 == % is direct label if index 0 == NULL is indirect label
	if (d->line.str[0] == '%' || !d->line.str[0])
		return (0);
	if (!(ft_strchrstr(LABEL_CHARS, d->line.str)))
		return (-1);
	tokenizer(d, LABEL, d->line.str, -1);
	d->line.pos += ft_strlen(d->line.str) + 1;
	ft_strclr(d->line.str);
	d->line.i = 0;
	return (1);
}
// check is instruction
int		is_instruction(t_all *d)
{
	int	i;
	int	cmp;

	i = 0;
	cmp = 1;
	while (op_tab[i].name)
		if (!(cmp = ft_strcmp(op_tab[i++].name, d->line.str)))
			break ;
	if (!cmp)
	{
		tokenizer(d, OPERATION, d->line.str, i - 1);
		d->line.pos += ft_strlen(d->line.str);
		ft_strclr(d->line.str);
		return (1);
	}
	return (0);
}

int		is_separator(t_all *d)
{
	if (d->line.str[0])
		ft_error_lixic(d);
	tokenizer(d, SEPARATOR, ",", -1);
	d->line.pos++;
	return (1);
}
// skip comment
int		is_comment(t_all *d)
{
	char	c;


	while (read(d->fd_assm, &c, 1) > 0)
	{
		if (c == '\n')
		{
			lseek(d->fd_assm, -1, SEEK_CUR);
			return (1);
		}
		d->line.pos++;
	}
	return (1);
}
// check number of _DIR
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
// check is _DIR
int		is_direct(t_all *d)
{
	if (d->line.str[1] == LABEL_CHAR)
	{
		if (!(ft_strchrstr(LABEL_CHARS, &d->line.str[2])))
			return (-1);
		tokenizer(d, _DIR_L, &d->line.str[2], -1);
	}
	else if (!digit_dir(&d->line.str[1]))
		return (-1);
	else
		tokenizer(d, _DIR, &d->line.str[1], -1);
	d->line.pos += ft_strlen(d->line.str);
	ft_strclr(d->line.str);
	d->line.i = 0;
	return (1);
}
//check is _REG
int		is_registr(t_all *d)
{
	if (!ft_all_digit(&d->line.str[1]))
		return (0);
	if (ft_atoi(&d->line.str[1]) > REG_NUMBER)
		return (-1);
	tokenizer(d, _REG, &d->line.str[1], -1);
	d->line.pos += ft_strlen(d->line.str);
	ft_strclr(d->line.str);
	return (1);
}
// check is _IND
int		is_indirect(t_all *d)
{
	if (d->line.str[0] == LABEL_CHAR)
	{
		if (!ft_strchrstr(LABEL_CHARS, &d->line.str[1]))
			return (-1);
		tokenizer(d, _IND_L, &d->line.str[1], -1);
	}
	else if (!digit_dir(d->line.str))
		return (-1);
	else
		tokenizer(d, _IND, d->line.str, -1);
	d->line.pos += ft_strlen(d->line.str);
	ft_strclr(d->line.str);
	d->line.i = 0;
	return (1);
}
// check is arguments _DIR | _IND | REG
int		is_arguments(t_all *d)
{
	int	ret;

	ret = 0;
	if (d->line.str[0] == DIR_CHAR)
		ret = is_direct(d);
	else if (d->line.str[0] == REG_CHAR)
		ret = is_registr(d);
	else if (d->line.str[0] == LABEL_CHAR || ft_isdigit(d->line.str[0]) || d->line.str[0] == '-')
		ret = is_indirect(d);
	return (ret);
}

int		check_token(t_all *d, char c)
{
	int	ret;

	ret = is_cmd(d, c);
	// check is instruction
	!ret && (c == ' ' || c == '\t' || c == '\n' || c == DIR_CHAR || c == ALT_COMMENT_CHAR || !c) ? ret = is_instruction(d) : 0;
	// check is label
	!ret &&  (c == LABEL_CHAR) ? ret = is_label(d) : 0;
	// check is argument like _DIR or _REG or _IND
	!ret && (c == SEPARATOR_CHAR || c == '\n' || c == ' ' || c == '\t' || !c) ? ret = is_arguments(d) : 0;
	// check is separator ","
	ret >= 0 && c == SEPARATOR_CHAR ? ret = is_separator(d) : 0;
	// check is space
	ret >= 0 && (c == '\t' || c == ' ') ? ret = is_spc(d) : 0;
	// check is string
	ret >= 0 && (c == '\"') ? ret = is_str(d) : 0;
	// check is end of line
	ret >= 0 && (c == '\n') ? ret = is_end(d) : 0;
	// check is comment start white "#" or ";"
	c == COMMENT_CHAR || c == ALT_COMMENT_CHAR ? ret = is_comment(d) : 0;
	ret == -1 ? ft_error_lixic(d) : 0;
	// if c == "%" and ret == 1 Ex: [ld%10, r3] no space butwen ld and %
	ret == 1 && c == DIR_CHAR ? lseek(d->fd_assm, -1, SEEK_CUR) : 0;
	return (ret);
}

void		print_d(t_all *d)//test
{
//	int i;
	t_token *tmp = d->token;
	while (tmp)
	{
		if (tmp->index_op >= 0 || tmp->type == LABEL)
		{
		ft_dprintf(1, "token name [%s] index_op[%d] possition [%03d %03d]", tmp->content, tmp->index_op, tmp->nbr_l, tmp->pos);
		//	i = 0;
		//	while (i < tmp->nbr_param)
		//		ft_printf(" |%d|", tmp->param_type[i++]);
		ft_dprintf(1, "  size = [%d]\n", tmp->ofset);
		}
		tmp = tmp->next;
	}
}

void		read_file(t_all *d)
{
	char	c;
	int	loop;

	loop = 1;
	ft_bzero(&d->line, sizeof(d->line));
	if (!(d->line.str = ft_memalloc(_BUFF)))
		error_malloc(d);
	d->line.nbr_l = 1;
	d->line.pos = 1;
	while (loop > 0)
	{
		if (!(loop = read(d->fd_assm, &c, 1)))
			c = 0;
		//check is define
		if (check_token(d, c))
			d->line.i = 0;
		else
			d->line.str[d->line.i++] = c;
	}
	ft_strdel(&d->line.str);
	//print_d(d);// test
}
