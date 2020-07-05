#include "assembler.h"

char		*convert_code(unsigned int val, int n)
{
	char	*str;
	int	i;

	i = 0;
	if (!(str = ft_memalloc(n)))
		return(NULL);
	while (n--)
		str[i++] = (val >> (n * 8) & 0xff);
	return (str);
}

void		head_to_buf(t_all *d)
{
	char	*str;

	(!(str = convert_code(COREWAR_EXEC_MAGIC, 4))) ? error_malloc(d) : 0;
	ft_memcpy((void *)d->buff, (void *)str, 4);
	d->pos += 4;
	ft_strdel(&str);
	ft_memcpy((void *)d->buff + d->pos, (void *)d->header->prog_name, PROG_NAME_LENGTH);
	d->pos += PROG_NAME_LENGTH;
	ft_memcpy((void *)d->buff + d->pos, "\0\0\0\0", 4);
	d->pos += 4;
	(!(str = convert_code(d->header->prog_size, 4))) ? error_malloc(d) : 0;
	ft_memcpy((void *)d->buff + d->pos, (void *)str, 4);
	ft_strdel(&str);
	d->pos += 4;
	ft_memcpy((void *)d->buff + d->pos, (void *)d->header->comment, COMMENT_LENGTH);
	d->pos += COMMENT_LENGTH;
	ft_memcpy((void *)d->buff + d->pos, "\0\0\0\0", 4);
	d->pos += 4;
	
}

void		print_buf(t_all *d)
{
	size_t	i;
	char	*f;

	i = 0;
	if (!(f = ft_strsub(d->name_exec, 0, ft_strlen(d->name_exec) - 1)))
		error_malloc(d);
	if (!(d->name_exec = ft_strjoin(f, "cor")))
	{
		free(f);
		error_malloc(d);
	}
	free(f);
	if ((d->fd_exec = open( d->name_exec, O_CREAT | O_WRONLY | O_TRUNC, 0777)) < 0)
	{
		ft_dprintf(1, "%s\n", strerror(errno));
		free_error_data(d);
		free(d->name_exec);
		exit(EXIT_FAILURE);
	}
	while (i < d->pos)
		write(d->fd_exec, &d->buff[i++], 1);
}

void		type_to_buf(t_all *d, t_instruction *ins)
{
	int		i;
	unsigned int	type;

	i = 0;
	type = 0;
	while (i < ins->nbr_param)
	{
		type |= ins->param_type[i] << ((MAX_OP_PARAMS - i) * 2);
		i++;
	}
	ft_memcpy((void *)d->buff + d->pos, &type, 1);
	d->pos++;
}

void		ins_to_buf(t_all *d)
{
	t_instruction	*ins;
	int		i;
	char		*ptr;

	ins = d->ins;
	while (ins)
	{
		ft_memcpy((void *)d->buff + d->pos, (char *)&ins->op_code, 1);
		d->pos++;
		ins->type ? type_to_buf(d, ins) : 0;
		i = 0;
		while (i < ins->nbr_param)
		{
			(!(ptr = convert_code(ins->param[i], ins->s_param[i]))) ? error_malloc(d) : 0;
			ft_memcpy((void *)d->buff + d->pos, (void *)ptr, ins->s_param[i]);
			d->pos += ins->s_param[i];
			ft_strdel(&ptr);
			i++;
		}
		ins = ins->next;
	}
}

void		buf_code(t_all *d)
{
	d->size += 4 + PROG_NAME_LENGTH + 4 + 4 + COMMENT_LENGTH + 4;
	(!(d->buff = ft_memalloc(d->size))) ? error_malloc(d) : 0;
	head_to_buf(d);
	ins_to_buf(d);
	print_buf(d);
}
