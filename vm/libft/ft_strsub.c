/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelazrak <yelazrak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/30 14:14:22 by yelazrak          #+#    #+#             */
/*   Updated: 2019/08/04 18:23:49 by yelazrak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*t;

	if (!s)
		return (NULL);
	if (start == ft_strlen(s))
		return (ft_strdup(""));
	if (!(t = ft_strnew(len)))
		return (NULL);
	t = ft_strncpy(t, s + start, len);
	return (t);
}
