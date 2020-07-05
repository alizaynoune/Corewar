#include "assembler.h"

t_token		*get_label(t_token *tk)
{
	while (tk)
	{
		if (tk->type == LABEL)
		{
			return (tk);
		}
		tk = tk->next;
	}
	return (NULL);
}

void		print_ins(t_all *d)
{
	t_label		*label;
	t_instruction	*ins;
	size_t		size;
	int		i;

	ins = d->ins;
	label = d->label;
	size = 0;
	while (ins)
	{
		if (label && label->ofset == size)
		{
			ft_dprintf(1, "(%d) %s:\n", label->ofset, label->name);
			label = label->next;
		}
		ft_dprintf(1, "%20s%d) %s", "(", ins->ofset, op_tab[ins->op_code - 1].name);
		size += ins->ofset;
		i = 0;
		while (i < ins->nbr_param)
			ft_dprintf(1, " %d", ins->param[i++]);
		ft_dprintf(1, "\n");
		ins = ins->next;
	}
}

int		value_label(t_all *d, t_instruction *ins, t_token *tk)
{
	t_label		*ptr;

	ptr = d->label;
	while (ptr)
	{
		if (!ft_strcmp(ptr->name, tk->content))
			return (ptr->ofset - d->size);
		ptr = ptr->next;
	}
	_unknown_label(d, ins, tk);
	return (0);
}

void		init_param(t_all *d, t_instruction *ins, t_token *tk)
{
	short	loop;
	int	i;
	short	s_dir;

	loop = tk->nbr_param;
	i = 0;
	s_dir = !op_tab[tk->index_op].s_dir ? DIR_SIZE : DIR_SIZE / 2;
	tk = tk->next;
	while (loop && tk)
	{
		if ((tk->type & (_REG | _DIR | _IND | _IND_L | _DIR_L)))
		{
			loop--;
			tk->type & (_DIR | _DIR_L) ? ins->param_type[i] = DIR_CODE : 0;
			tk->type & (_IND | _IND_L) ? ins->param_type[i] = IND_CODE : 0;
			tk->type & _REG ? ins->param_type[i] = REG_CODE : 0;
			tk->type & (_DIR | _IND | _REG) ? ins->param[i] = ft_atoi(tk->content) : 0;
			tk->type & (_DIR_L | _IND_L) ? ins->param[i] = value_label(d, ins, tk) : 0;
			tk->type & (_DIR_L | _DIR) ? ins->s_param[i] = s_dir : 0;
			tk->type & (_IND | _IND_L) ? ins->s_param[i] = IND_SIZE : 0;
			tk->type & (_REG) ? ins->s_param[i] = REG_SIZE / 4 : 0;
			i++;
		}
		tk = tk->next;
	}
}

void		push_ins(t_instruction *new, t_all *d)
{
	t_instruction	*ptr;

	ptr = d->ins;
	if (d->ins)
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
	else
		d->ins = new;
}

void		init_ins(t_token *tk, t_all *d)
{
	t_instruction	*new;

	(!(new = ft_memalloc(sizeof(t_instruction)))) ? error_malloc(d) : 0;
	new->op_code = op_tab[tk->index_op].cod_op;
	new->type = op_tab[tk->index_op].arg_type;
	new->ofset = tk->ofset;
	init_param(d, new, tk);
	new->nbr_param = tk->nbr_param;
	d->ptr_token = d->ptr_token->next;
	d->size += new->ofset;
	push_ins(new, d);
}

void		init_code(t_all *d)
{
	d->ptr_token = d->token;
	while (d->ptr_token)
		d->ptr_token->index_op >= 0 ? init_ins(d->ptr_token, d) : (d->ptr_token = d->ptr_token->next);
//	print_ins(d);
	free_token(d->token);
	d->token = NULL;
}
