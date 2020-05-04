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
# include "ft_printf.h"

# define PROG_NAME_LENGTH	(128)
# define COMMENT_LENGTH		(2048)
# define MAX_OP_PARAMS		(3)

/*
 **	Codes for possible types of tokens
 **
 **	(1 << 0) `label:`
 **	(1 << 1) `live|ld|...`
 **	(1 << 2) `r[unsigned numeric literal]`
 **	(1 << 3) `,`
 **	(1 << 4) `%:label`
 **	(1 << 5) `%[numeric literal]`
 **	(1 << 6) `numeric literal`
 **	(1 << 7) `:label`
 **	(1 << 8) `Whitespaces`
 **	(1 << 9) `String literal`
 */

#define _REG			(1)
#define _DIR			(2)
#define _IND			(4)
#define _DIR_L			(8)
#define _IND_L			(16)
#define LABEL			(32)
#define OPERATION		(64)
#define SEPARATOR		(128)
#define	STRING			(256)
#define COMM			(512)
#define END			(1024)

typedef struct		s_arg
{
	int		type;
	int		value;
	int8_t		separator;
	struct s_arg	*next;
}			t_arg;

typedef struct			s_instruction
{
	char			*lebl;
	short			nb_arg;
	int			intstruct;
	t_arg			*arg;
	struct s_instruction	*next;
}				t_instruction;

typedef struct		s_token
{
	int		type;//stores the code of the type
	char		*content;
	size_t		n_l;
	size_t		n_c;
	int		n_op;
	struct s_token	*next;
}			t_token;

typedef struct		s_line
{
	char		*str;
	size_t		nb_line;
	size_t		n_l;
	size_t		n_c;
	size_t		i;
}			t_line;

typedef struct			s_operation
{
	char			*name;
	short			n_arg;
	short			arg1;
	short			arg2;
	short			arg3;
}				t_operation;

typedef struct		s_data
{
	t_operation	*op;
	int		fd_assm;
	int		fd_exec;
	header_t	header;
	t_line		line;
	t_token		*token;
	t_instruction	*instruction;
}			t_data;

/*typedef struct		s_instruction
  {
  char			*label;
  char			*operation;
  short			nparams;
  unsigned char	params[MAX_OP_PARAMS];	
  int				size;
  }					t_instruction;

  typedef struct		s_op
  {
  char			*operation;
  short			nparams;
  unsigned char	params[MAX_OP_PARAMS];	
  unsigned char	opcode;
  short			exec_cycle;
  char			*description;
//... 2 other fields;
}					t_op;
*/
void		read_file(t_data *d);
void		ft_error_lixic(t_data *d);
void		error_malloc(t_data *d);
void		message_exit(char *str);
void		free_data(t_data *d);
void		ft_syntax(t_data *d);
#endif
