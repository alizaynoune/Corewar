#include "assembler.h"

void		ft_error_lixic(t_all *d)
{
	ft_dprintf(1, "Lexical error at [%03d %03d] \"%s\"\n", d->line.nbr_l, d->line.pos, d->line.str);
	free_error_data(d);
	ft_strdel(&d->line.str);
	exit (EXIT_FAILURE);
}

void		_unknown_label(t_all *d, t_instruction *ptr, t_token *tk)
{
	free(ptr);
	ft_dprintf(1, "unknown label at toke [%03d : %03d]\n", tk->nbr_l, tk->pos);
	free_error_data(d);
	exit (EXIT_FAILURE);
}
