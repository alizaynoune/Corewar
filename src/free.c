#include "assembler.h"

void		error_malloc(t_data *d)
{
	d->line.str != NULL ? ft_strdel(&d->line.str): 0;
	close(d->fd_assm);
	free_data(d);
	message_exit("Error malloc");
}

void		free_data(t_data *d)
{
	t_token		*tmp;

	while (d->token)
	{
		tmp = d->token;
		d->token = d->token->next;
		ft_strdel(&tmp->content);
		free(tmp);
	}
	free(d);
}
