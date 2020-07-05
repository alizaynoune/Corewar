#include "assembler.h"

void		error_malloc(t_all *d)
{
	d->line.str != NULL ? ft_strdel(&d->line.str): 0;
	free_error_data(d);
	message_exit("Error malloc");
}

void		free_label(t_label *label)
{
	t_label		*ptr;

	while (label)
	{
		ptr = label;
		label = label->next;
		free(ptr->name);
		free(ptr);
	}
}

void		free_ins(t_instruction *ins)
{
	t_instruction	*ptr;

	while (ins)
	{
		ptr = ins;
		ins = ins->next;
		free(ptr);
	}
}

void		free_token(t_token *token)
{
	t_token		*ptr;

	while (token)
	{
		ptr = token;
		token = token->next;
		ft_strdel(&ptr->content);
		free(ptr);
	}
}

void		free_error_data(t_all *d)
{
	close(d->fd_assm);
	d->buff ? free(d->buff) : 0;
//	free_token(d->token);
//	(d->header) ? free(d->header) : 0;
	(d->label) ? free_label(d->label) : 0;
	(d->ins) ? free_ins(d->ins) : 0;
	free(d);
}

void		free_success(t_all *d)
{
	close(d->fd_assm);
	close(d->fd_exec);
	free(d->name_exec);
	free(d->buff);
	free(d->header);
	free_label(d->label);
	free_ins(d->ins);
	free(d);
}
