#include "assembler.h"
// print right syntax if it's error syntax
void		print_type_arg(int arg)
{
	arg == _REG ? ft_dprintf(1, "REG"): 0;
	arg == _DIR ? ft_dprintf(1, "DIR"): 0;
	arg == (_REG | _DIR) ? ft_dprintf(1, "REG | DIR"): 0;
	arg == _IND ? ft_dprintf(1, "IND"): 0;
	arg == (_REG | _IND) ? ft_dprintf(1, "REG | IND"): 0;
	arg == (_IND | _DIR) ? ft_dprintf(1, "IND | DIR"): 0;
	arg == (_REG | _DIR | _IND) ? ft_dprintf(1, "REG | DIR | IND"): 0;
}
// print right syntax if it's error syntax
void		print_right_syntax(t_operation op)
{
	int	i;

	i = 0;
	ft_dprintf(1, "  => The right arg's syntax of \"%s\" is [ ",op.name);
	while (i < op.n_arg)
	{
		print_type_arg(op.args[i++]);
		op.args[i] > 0 && i < op.n_arg ? ft_dprintf(1, ", "): 0;
	}
	ft_dprintf(1, " ]");
}

void		error_syntax(t_token *tk, t_all *d)
{
	if (tk)
		ft_dprintf(1, "Syntax error at token [%03d %03d]", tk->nbr_l, tk->pos);
//	if (tk && tk->index_op >= 0)
//		print_right_syntax(op_tab[tk->index_op]);
	ft_dprintf(1, "\n");
	free_error_data(d);
	exit(EXIT_FAILURE);
}
// end of line not fond
void		error_end(t_token *tk, t_all *d, t_token *tmp)
{
	if (!tmp)
		ft_dprintf(1, "Syntax error you forgot newline in end of file\n");
	else
		error_syntax(tk, d);
	free_error_data(d);
	exit(EXIT_FAILURE);
}

int		error_args(int arg, t_operation op, t_token *tk, int i)
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
	if (op.args[i] && (op.args[i] & ar))
	{
		//tk->param_type[i] = ar;
		tk->nbr_param = op.n_arg;
		if (ar == _DIR)
			tk->ofset += !op.s_dir ? DIR_SIZE : DIR_SIZE / 2;
		tk->ofset += (ar == _IND) ? IND_SIZE : 0;
		tk->ofset += (ar == _REG) ? REG_SIZE / 4 : 0;
		return (1);
	}
	return (0);
}
// loop for all args after each instruction
t_token		*ft_args(t_token *tk, t_all *d)
{
	int	i;
	t_token	*tmp;

	i = 0;
	tmp = tk->next;
	tk->ofset = op_tab[tk->index_op].arg_type ? 2 : 1;
	while (tmp && i < op_tab[tk->index_op].n_arg && tmp->type != END)
	{
		// check if arg is deifine in this instruction
		if (tmp->type != SEPARATOR && !error_args(tmp->type, op_tab[tk->index_op], tk, i++))
			error_syntax(tk, d);
		tmp = tmp->next;
		// after each arg will fined separator or end of line else error
		if (tmp && tmp->type != END && tmp->type != SEPARATOR)
			error_syntax(tk, d);
		if (tmp && tmp->type == SEPARATOR)
			tmp = tmp->next;
	}
	// if number of args in this instruction don't courrect
	if (i < op_tab[tk->index_op].n_arg)
		error_syntax(tk, d);
	// end of line not fond
	if (!tmp || tmp->type != END)
		error_end(tk, d, tmp);
	d->header->prog_size += tk->ofset;
	return (tmp);
}
// if comment or name string is long
void		cmd_so_long(char *str, t_all *d)
{
	ft_dprintf(1, "%s is so long\n", str);
	free_error_data(d);
	exit(EXIT_FAILURE);
}
// copy name and comment from token to my struct in header
t_token		*ft_copy_cmd(t_token *tk, t_all *d)
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
t_token		*ft_get_cmd(t_all *d)
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
void		free_cmd_from_list(t_all *d, t_token *tk)
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

void		push_label(t_all *d, t_token *tk)
{
	t_label		*new;
	t_label		*ptr;

	(!(new = ft_memalloc(sizeof(t_label)))) ? error_malloc(d) : 0;
	if (!(new->name = ft_strdup(tk->content)))
	{
		free(new);
		error_malloc(d);
	}
	new->ofset = d->header->prog_size;
	if (d->label)
	{
		ptr = d->label;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
	else
		d->label = new;
}

void		ft_syntax(t_all *d)
{
	t_token		*tk;

	// get comment and name
	tk = ft_get_cmd(d);
	// free token of name and comment
	free_cmd_from_list(d, tk);
	while (tk)
	{
		// if index_op >= 0 so it's an instruction
		if (tk->index_op >= 0)
			tk = ft_args(tk, d);
		// after each instruction will finde end of line else error
		else if(tk && tk->type != END && tk->type != LABEL)
			error_syntax(tk, d);
		tk->type == LABEL ? push_label(d, tk) : 0;
		tk = tk->next;
	}
}
