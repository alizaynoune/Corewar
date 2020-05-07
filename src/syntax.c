#include "assembler.h"

void		print_code_arg(int arg)
{
	arg == 1 ? ft_printf("REG"): 0;
	arg == 2 ? ft_printf("DIR"): 0;
	arg == 3 ? ft_printf("REG | DIR"): 0;
	arg == 4 ? ft_printf("IND"): 0;
	arg == 5 ? ft_printf("REG | IND"): 0;
	arg == 6 ? ft_printf("IND | DIR"): 0;
	arg == 7 ? ft_printf("REG | DIR | IND"): 0;
}

void		print_right_syntax(t_operation op)
{
	ft_printf("  => The right syntax of \"%s\" is [ ",op.name);
	op.arg1 > 0 ? print_code_arg(op.arg1): 0;
	op.arg2 > 0 ? ft_printf(", "): 0;
	op.arg2 > 0 ? print_code_arg(op.arg2): 0;
	op.arg3 > 0 ? ft_printf(", "): 0;
	op.arg3 > 0 ? print_code_arg(op.arg3): 0;
	ft_printf(" ]");
}

void		error_syntax(t_token *tk, t_data *d)
{
	if (tk)
		ft_printf("Syntax error at token [%03d %03d]", tk->n_l, tk->n_c);
	if (tk && tk->n_op >= 0)
		print_right_syntax(d->op[tk->n_op]);
	ft_printf("\n");
	free_data(d);
	exit(1);
}

void		error_end(t_token *tk, t_data *d, t_token *tmp)
{
	if (!tmp)
		ft_printf("Syntax error you forgot newline in end of file\n");
	else
		error_syntax(tk, d);
	free_data(d);
	exit(1);
}

int		chek_arg(int op, int arg)
{
	int	cmp;

	cmp = op - arg;
	if (cmp >= 0 && cmp <= 6 && cmp != arg)
		return (1);
	return (0);
}

int		error_args(int arg, t_operation op, int i)
{
	int	ar;

	if (arg == _DIR_L)
		ar = _DIR;
	else if (arg == _IND_L)
		ar = _IND;
	else
		ar = arg;
	if (i == 0)
	       return (chek_arg(op.arg1, ar));
	if (i == 1)
		return (chek_arg(op.arg2, ar));
	if (i == 2)
		return (chek_arg(op.arg3, ar));
	return (0);
}

t_token		*ft_args(t_token *tk, t_data *d)
{
	int	i;
	t_token	*tmp;

	i = 0;
	tmp = tk->next;
	while (tmp && i < d->op[tk->n_op].n_arg && tmp->type != END)
	{
		if (tmp->type != SEPARATOR && !error_args(tmp->type, d->op[tk->n_op], i++))
			error_syntax(tk, d);
		tmp = tmp->next;
		if (tmp && tmp->type != END && tmp->type != SEPARATOR)
			error_syntax(tk, d);
		if (tmp && tmp->type == SEPARATOR)
			tmp = tmp->next;
	}
	if (i < d->op[tk->n_op].n_arg)
		error_syntax(tk, d);
	if (!tmp || tmp->type != END)
		error_end(tk, d, tmp);
	return (tmp);
}

void		cmd_so_long(char *str, t_data *d)
{
	ft_printf("%s is so long\n", str);
	free_data(d);
	exit(1);
}

t_token		*ft_copy_cmd(t_token *tk, t_data *d)
{
	char	*type;
	size_t	i;

	i = -1;
	type = tk->content;
	tk = tk->next;
	tk->type != STRING ? error_syntax(tk, d): 0;
	if (!ft_strcmp(type, NAME_CMD_STRING))
	{
		while (++i < PROG_NAME_LENGTH && tk->content[i])
			d->header->prog_name[i] = tk->content[i];
		if (ft_strlen(tk->content) > PROG_NAME_LENGTH)
			cmd_so_long("Prog_name", d);
	}
	else if (!ft_strcmp(type, COMMENT_CMD_STRING))
	{
		while (++i < COMMENT_LENGTH && tk->content[i])
			d->header->comment[i] = tk->content[i];
		if (ft_strlen(tk->content) > COMMENT_LENGTH)
			cmd_so_long("Comment", d);
	}
	tk = tk->next;
	tk == NULL || tk->type != END ? error_syntax(tk, d): 0;
	while (tk->next && tk->type == END)
		tk = tk->next;
	tk->next == NULL && tk->type == END ? error_syntax(tk, d): 0;
	return (tk);
}

t_token		*ft_get_cmd(t_data *d)
{
	t_token		*tk;

	tk = d->token;
	while (tk && tk->type == END)
		tk = tk->next;
	tk && tk->type == COMM ? tk = ft_copy_cmd(tk, d): error_syntax(tk, d);
	while (tk && tk->type == END)
		tk = tk->next;
	tk && tk->type == COMM ? tk = ft_copy_cmd(tk, d): error_syntax(tk, d);
	return (tk);
}

void		free_cmd_from_list(t_data *d, t_token *tk)
{
	t_token		*tmp;

	while (d->token != tk)
	{
		tmp = d->token;
		d->token = d->token->next;
		free(tmp->content);
		free(tmp);
	}
}

void		ft_syntax(t_data *d)
{
	t_token		*tk;

//	print_d(d);
	tk = ft_get_cmd(d);
	free_cmd_from_list(d, tk);
	while (tk)
	{
		if (tk->n_op >= 0)
			tk = ft_args(tk, d);
		else if(tk && tk->type != END && tk->type != LABEL)
			error_syntax(tk, d);
		tk = tk->next;
	}
//	ft_printf("name [%s]\ncomment [%s]\n", d->header->prog_name, d->header->comment);
}
