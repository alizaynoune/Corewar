#include "assembler.h"
// print right syntax if it's error syntax
void		print_code_arg(int arg)
{
	arg == _REG ? ft_printf("REG"): 0;
	arg == _DIR ? ft_printf("DIR"): 0;
	arg == _REG + _DIR ? ft_printf("REG | DIR"): 0;
	arg == _IND ? ft_printf("IND"): 0;
	arg == _REG + _IND ? ft_printf("REG | IND"): 0;
	arg == _IND + _DIR ? ft_printf("IND | DIR"): 0;
	arg == _REG + _DIR + _IND ? ft_printf("REG | DIR | IND"): 0;
}
// print right syntax if it's error syntax
void		print_right_syntax(t_operation op)
{
	int	i;

	i = 0;
	ft_printf("  => The right arg's syntax of \"%s\" is [ ",op.name);
	while (op.args[i])
	{
		print_code_arg(op.args[i++]);
		op.args[i] > 0 && i < op.n_arg ? ft_printf(", "): 0;
	}
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
// end of line not fond
void		error_end(t_token *tk, t_data *d, t_token *tmp)
{
	if (!tmp)
		ft_printf("Syntax error you forgot newline in end of file\n");
	else
		error_syntax(tk, d);
	free_data(d);
	exit(1);
}
// check arg is define in instruction
int		check_arg(int op, int arg)
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

	// convert direct label to direct
	if (arg == _DIR_L)
		ar = _DIR;
	// convert indirect lable to indirect
	else if (arg == _IND_L)
		ar = _IND;
	else
		ar = arg;
	// check this arg are define in this instruction
	if (op.args[i])
		return (check_arg(op.args[i], ar));
	return (0);
}
// loop for all args after each instruction
t_token		*ft_args(t_token *tk, t_data *d)
{
	int	i;
	t_token	*tmp;

	i = 0;
	tmp = tk->next;
	while (tmp && i < d->op[tk->n_op].n_arg && tmp->type != END)
	{
		// check if arg is deifine in this instruction
		if (tmp->type != SEPARATOR && !error_args(tmp->type, d->op[tk->n_op], i++))
			error_syntax(tk, d);
		tmp = tmp->next;
		// after each arg will fined separator or end of line else error
		if (tmp && tmp->type != END && tmp->type != SEPARATOR)
			error_syntax(tk, d);
		if (tmp && tmp->type == SEPARATOR)
			tmp = tmp->next;
	}
	// if number of args in this instruction don't courrect
	if (i < d->op[tk->n_op].n_arg)
		error_syntax(tk, d);
	// end of line not fond
	if (!tmp || tmp->type != END)
		error_end(tk, d, tmp);
	return (tmp);
}
// if comment or name string is long
void		cmd_so_long(char *str, t_data *d)
{
	ft_printf("%s is so long\n", str);
	free_data(d);
	exit(1);
}
// copy name and comment from token to my struct in header
t_token		*ft_copy_cmd(t_token *tk, t_data *d)
{
	char	*type;
	size_t	i;

	i = -1;
	type = tk->content;
	tk = tk->next;
	tk->type != STRING ? error_syntax(tk, d): 0;
	// copy name
	if (!ft_strcmp(type, NAME_CMD_STRING))
	{
		while (++i < PROG_NAME_LENGTH && tk->content[i])
			d->header->prog_name[i] = tk->content[i];
		if (ft_strlen(tk->content) > PROG_NAME_LENGTH)
			cmd_so_long("Prog_name", d);
	}
	// copy comment
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
// get name and comment
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
// free comment and name
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
	// get comment and name
	tk = ft_get_cmd(d);
	// free token of name and comment
	free_cmd_from_list(d, tk);
	while (tk)
	{
		// if n_op >= 0 so it's a instruction
		if (tk->n_op >= 0)
			tk = ft_args(tk, d);
		// after each instruction will finde end of line else error
		else if(tk && tk->type != END && tk->type != LABEL)
			error_syntax(tk, d);
		tk = tk->next;
	}
//	ft_printf("name [%s]\ncomment [%s]\n", d->header->prog_name, d->header->comment);
}
