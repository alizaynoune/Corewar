/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assembler.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhossan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 16:34:57 by akhossan          #+#    #+#             */
/*   Updated: 2020/03/12 20:58:17 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASSEMBLER_H
# define ASSEMBLER_H

# include <errno.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include "op.h"
# include "ft_dprintf.h"

# define MAX_OP_PARAMS		(3)

#define _REG			(1)
#define _IND			(2)
#define _DIR			(4)
#define _DIR_L			(8)
#define _IND_L			(16)
#define LABEL			(32)
#define OPERATION		(64)
#define SEPARATOR		(128)
#define	STRING			(256)
#define COMM			(512)
#define END			(1024)

#define _BUFF			(10)

typedef struct		s_token
{
	int		type;//stores the code of the type
	char		*content;
	size_t		nbr_l;
	size_t		pos;
	int		index_op;
	short		nbr_param;
	size_t		ofset;
	struct s_token	*next;
}			t_token;

typedef struct		s_line
{
	char		*str;
	size_t		nb_line;
	size_t		nbr_l;
	size_t		pos;
	size_t		i;
	size_t		size;
}			t_line;

typedef struct			s_operation
{
	char			*name;
	short			n_arg;
	short			args[MAX_OP_PARAMS];
	short			cod_op;
	short			cycles;
	char			*desc;
	short			arg_type;
	short			s_dir;
}				t_operation;

typedef struct			s_instruction
{
	short			op_code;
	unsigned int		param[MAX_OP_PARAMS];
	short			param_type[MAX_OP_PARAMS];
	short			s_param[MAX_OP_PARAMS];
	short			type;
	short			nbr_param;
	unsigned int		ofset;
	struct s_instruction	*next;
}				t_instruction;

typedef struct			s_label
{
	char			*name;
	unsigned int		ofset;
	struct s_label		*next;
}				t_label;

typedef struct		s_all
{
	int		fd_assm;
	int		fd_exec;
	char		*name_exec;
	header_t	*header;
	t_line		line;
	t_token		*token;
	t_token		*ptr_token;
	t_label		*label;
	t_instruction	*ins;
	size_t		size;
	char		*buff;
	size_t		pos;
}			t_all;

extern t_operation	op_tab[];

void		read_file(t_all *d);
void		ft_error_lixic(t_all *d);
void		error_malloc(t_all *d);
void		message_exit(char *str);
void		free_error_data(t_all *d);
void		free_token(t_token *token);
void		ft_syntax(t_all *d);
void		print_d(t_all *d); //test
void		init_code(t_all *d);
void		_unknown_label(t_all *d, t_instruction *ptr, t_token *tk);
void		buf_code(t_all *d);
void		free_success(t_all *d);
#endif
