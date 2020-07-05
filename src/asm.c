#include "assembler.h"
t_operation    op_tab[17] =
{
	{"live", 1, {_DIR}, 1, 10, "alive", 0, 0},
	{"ld", 2, {_DIR | _IND, _REG}, 2, 5, "load", 1, 0},
	{"st", 2, {_REG, _IND | _REG}, 3, 5, "store", 1, 0},
	{"add", 3, {_REG, _REG, _REG}, 4, 10, "addition", 1, 0},
	{"sub", 3, {_REG, _REG, _REG}, 5, 10, "soustraction", 1, 0},
	{"and", 3, {_REG | _DIR | _IND, _REG | _IND | _DIR, _REG}, 6, 6,
		"et (and  r1, r2, r3   r1&r2 -> r3", 1, 0},
	{"or", 3, {_REG | _IND | _DIR, _REG | _IND | _DIR, _REG}, 7, 6,
		"ou  (or   r1, r2, r3   r1 | r2 -> r3", 1, 0},
	{"xor", 3, {_REG | _IND | _DIR, _REG | _IND | _DIR, _REG}, 8, 6,
		"ou (xor  r1, r2, r3   r1^r2 -> r3", 1, 0},
	{"zjmp", 1, {_DIR}, 9, 20, "jump if zero", 0, 1},
	{"ldi", 3, {_REG | _DIR | _IND, _DIR | _REG, _REG}, 10, 25,
		"load index", 1, 1},
	{"sti", 3, {_REG, _REG | _DIR | _IND, _DIR | _REG}, 11, 25,
		"store index", 1, 1},
	{"fork", 1, {_DIR}, 12, 800, "fork", 0, 1},
	{"lld", 2, {_DIR | _IND, _REG}, 13, 10, "long load", 1, 0},
	{"lldi", 3, {_REG | _DIR | _IND, _DIR | _REG, _REG}, 14, 50,
		"long load index", 1, 1},
	{"lfork", 1, {_DIR}, 15, 1000, "long fork", 0, 1},
	{"aff", 1, {_REG}, 16, 2, "aff", 1, 0},
	{0, 0, {0}, 0, 0, 0, 0, 0}
};

void		message_exit(char *str)
{
	ft_dprintf(2, "%s\n", str);
	exit(EXIT_FAILURE);
}

void		error_file(t_all *d, char *msg)
{
	if (d->fd_assm)
		close(d->fd_assm);
	free(d);
	message_exit(msg);
}

void		ft_open_file(t_all *d, char *f)
{
	char	c;
	int	rd;
	size_t	len;

	len = ft_strlen(f);
	if ((d->fd_assm = open(f, O_RDONLY)) < 0)
		error_file(d, strerror(errno));
	if ((rd = read(d->fd_assm, &c, 1)) < 0)
		error_file(d, strerror(errno));
	if (len <= 2 || !ft_strnstr(&f[len - 2], ".s", 2))
		error_file(d, "Usage: ./asm <sorcefile.s>");
	rd == 0 ? error_file(d, "File is empty") : 0;
	lseek(d->fd_assm, SEEK_SET, 0);
}

int		main(int ac, char **av)
{
	t_all		*data;

	data = NULL;
	if (ac < 2)
		message_exit("Usage: ./asm <sorcefile.s>");
	if (!(data = ft_memalloc(sizeof(t_all))))
		message_exit("Error malloc");
	if (!(data->header = ft_memalloc(sizeof(header_t))))
		error_malloc(data);
	ft_open_file(data, av[ac - 1]);
	// read file and check lexical
	data->name_exec = av[ac - 1];
	read_file(data);
	// check syntax
	ft_syntax(data);
	init_code(data);
	buf_code(data);
	ft_dprintf(1, "Writing output program to %s\n", data->name_exec);
	free_success(data);
	return (1);
}
