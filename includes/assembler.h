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
//# include "op.h"
# include "../libprintf/includes/ft_printf.h"

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

#define LABEL_T				(0x01)
#define OPERATION_T			(0x02)
#define REGISTRY_T			(0x04)
#define SEPARATOR_T			(0x08)
#define DIRECT_LABEL_T		(0x10)
#define DIRECT_T			(0x20)
#define INDIRECT_T			(0x40)
#define INDIRECT_LABEL_T	(0x80)
#define	WHITESPACE_T		(0x100)
#define	STRING_LITERAL_T	(0x200)

typedef struct		s_line
{
	char			*line;
	unsigned int	nb_line;
}					t_line;

typedef struct      s_header
{
	unsigned int    magic;
	char            prog_name[PROG_NAME_LENGTH + 1];
	unsigned int    prog_size;
	char            comment[COMMENT_LENGTH + 1];
}                   t_header;

typedef struct		s_arg
{
	int				type;
	int				value;
	int8_t			separator;
	struct s_arg	*next;
}					t_arg;

typedef struct				s_instruction
{
	char					*lebl;
	short					nb_arg;
	int						intstruct;
	t_arg					*arg;
	struct s_instruction	*next;
}							t_instruction;

typedef struct		s_data
{
	int				fd_assm;
	int				fd_exec;
	t_header		header;
	t_line			lines;
	t_instruction	*instruction;
}					t_data;

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

typedef struct		s_token
{
	char			type;//stores the code of the type
	char			*content;
	struct s_token	*next;
}					t_token;
*/
#endif
