#include "assembler.h"

void		error_syntax(t_token *tk, t_data *d)
{
	
	if (tk)
		ft_printf("Syntax error at token [%03d %03d]", tk->n_l, tk->n_c);
	if (tk && tk->n_op >= 0)
		ft_printf(" =>%s", d->op[tk->n_op].name);
	if (tk && d)
		ft_printf("\n");
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
	while (i < d->op[tk->n_op].n_arg)
	{
		if (tmp->type != SEPARATOR && !error_args(tmp->type, d->op[tk->n_op], i++))
			error_syntax(tk, d);
		tmp = tmp->next;
	}
	if (tmp->type != END)
		error_syntax(tk, d);
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

void		ft_syntax(t_data *d)
{
	t_token		*tk;

	tk = ft_get_cmd(d);
	while (tk)
	{
		if (tk->n_op >= 0)
			tk = ft_args(tk, d);
		else if(tk && tk->type != END && tk->type != LABEL)
		{
			error_syntax(tk, d);
			ft_printf("ff:\n");
		}
		tk = tk->next;
	}
//	ft_printf("name [%s]\ncomment [%s]\n", d->header->prog_name, d->header->comment);
}
