#include "assembler.h"

void		error_syntax(t_token *tk, t_data *d)
{
	ft_printf("Syntax error at token [%03d %03d]", tk->n_l, tk->n_c);
	if (tk->n_op >= 0)
		ft_printf(" =>%s", d->op[tk->n_op].name);
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
	//ft_printf("%d %d\n", arg, op);
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

void		ft_syntax(t_data *d)
{
	t_token		*tk;

	tk = d->token;
	while (tk)
	{
		if (tk->n_op >= 0)
			tk = ft_args(tk, d);
	//	else
	//		error_syntax(tk, d);
	//	if (tk)
			tk = tk->next;
	}
}
