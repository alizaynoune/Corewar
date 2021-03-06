/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelazrak <yelazrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 13:07:19 by yelazrak          #+#    #+#             */
/*   Updated: 2020/12/01 12:10:02 by yelazrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/corewar.h"

int					ft_fork(t_process *p)
{
	unsigned	int	byt;
	t_corewar		*war;
	t_process		*new;

	war = get_corewar(0);
	ft_memcpy((void *)&byt, (void *)&war->arena[p->pc + 1], 2);
	if (!(new = (t_process *)malloc(sizeof(t_process))))
		return (0);
	ft_memset((void *)new, 0, sizeof(t_process));
	ft_memcpy((void *)new, (void *)p, sizeof(t_process));
	war->nbr_process++;
	new->pc = overrided_pos(ft_sign(byt, 2), p->pc);
	new->cycle_create = war->cycle;
	new->cycle_count = 0;
	new->next = NULL;
	new->next = war->all_process;
	war->all_process = new;
	p->pc += 3;
	p->pc %= MEM_SIZE;
	return (0);
}
