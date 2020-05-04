#include "assembler.h"

void		ft_error_lixic(t_data *d)
{
	ft_printf("Lexical error at [%03d %03d] \"%s\"\n", d->line.n_l, d->line.n_c, d->line.str);
	exit (1); //free
}
