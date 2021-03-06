/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalata <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/24 17:34:22 by ysalata           #+#    #+#             */
/*   Updated: 2016/12/05 12:44:21 by ysalata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strcat(char *s1, const char *s2)
{
	int i;
	int n;

	n = ft_strlen(s1);
	i = -1;
	while (s2[++i])
		s1[i + n] = s2[i];
	s1[i + n] = '\0';
	return (s1);
}
