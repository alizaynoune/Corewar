#include "assembler.h"

t_operation	op[17] =
{
	{"live", 1, {_DIR, 0, 0}},
	{"ld", 2, {_DIR | _IND, _REG, 0}},
	{"st", 2, {_REG, _REG | _IND, 0}},
	{"add", 3, {_REG, _REG, _REG}},
	{"sub", 3, {_REG, _REG, _REG}},
	{"and", 3, {_REG | _DIR | _IND, _IND | _REG | _DIR, _REG}},
	{"or", 3, {_REG | _IND | _DIR, _REG | _IND | _DIR, _REG}},
	{"xor", 3, {_REG | _IND | _DIR, _REG | _IND | _DIR, _REG}},
	{"zjmp", 1, {_DIR, 0, 0}},
	{"ldi", 3, {_REG | _DIR | _IND, _DIR | _REG, _REG}},
	{"sti", 3, {_REG, _REG | _DIR | _IND, _DIR | _REG}},
	{"fork", 1, {_DIR, 0, 0}},
	{"lld", 2, {_DIR | _IND, _REG, 0}},
	{"lldi", 3, {_REG | _DIR | _IND, _DIR | _REG, _REG}},
	{"lfork", 1, {_DIR, 0, 0}},
	{"aff", 1, {_REG, 0, 0}},
	{NULL, 0, {0, 0, 0}}
};

void		message_exit(char *str)
{
	ft_printf("%s\n", str);
	exit(1);
}

void		error_file(t_data *d, char *msg)
{
	if (d->fd_assm)
		close(d->fd_assm);
	free(d);
	message_exit(msg);
}

void		ft_open_file(t_data *d, char *f)
{
	char	c;
	int	rd;

	if ((d->fd_assm = open(f, O_RDONLY)) < 0)
		error_file(d, strerror(errno));
	if ((rd = read(d->fd_assm, &c, 1)) < 0)
		error_file(d, strerror(errno));
	rd == 0 ? error_file(d, "File is empty") : 0;
	lseek(d->fd_assm, SEEK_SET, 0);
}

int		main(int ac, char **av)
{
	t_data		*data;

	data = NULL;
	if (ac < 2)
		message_exit("Usage: ./asm <sorcefile>");
	if (!(data = ft_memalloc(sizeof(t_data))))
		message_exit("Error malloc");
	if (!(data->header = ft_memalloc(sizeof(header_t))))
		error_malloc(data);
	data->op = op;
	ft_open_file(data, av[ac - 1]);
	// read file and check lexical
	read_file(data);
	// check syntax
	ft_syntax(data);
	//print_d(data);
	free_data(data);
	ft_printf("Successfuly\n");
	return (1);
}
