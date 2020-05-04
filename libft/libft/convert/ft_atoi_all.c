/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 00:38:57 by alzaynou          #+#    #+#             */
/*   Updated: 2020/01/20 02:33:03 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_atoi_all(char *str)
{
	int		sing;
	int		num;
	int		cnt;

	cnt = ft_skip_char(str, "\t ");
	num = 0;
	if (!str)
		return (0);
	sing = str[cnt] == '-' ? -1 : 1;
	if (str[cnt] == '-' || str[cnt] == '+')
		cnt++;
	while (str[cnt] && str[cnt] >= '0' && str[cnt] <= '9')
	{
		num = (num * 10) + (str[cnt] - '0');
		cnt++;
	}
	cnt += ft_skip_char(&str[cnt], "\t ");
	num = !str[cnt] ? num * sing : 0;
	return (num);
}
