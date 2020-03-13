/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:03:07 by alzaynou          #+#    #+#             */
/*   Updated: 2020/03/12 14:41:34 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# define BUFF_SIZE 100

int				get_next_line(const int fd, char **line);

typedef struct	s_node
{
	char			*str;
	int				nfd;
	struct s_node	*next;
}				t_node;

#endif
