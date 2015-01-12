/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmeier <nmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 18:14:56 by nmeier            #+#    #+#             */
/*   Updated: 2014/11/08 17:46:17 by nmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

char *strjoin(char const *s1, char const *s2)
{
	int		s1_len;
	int		s2_len;
	char	*result;
	int		i;

	if (!s1 || !s2)
		return (NULL);
	s1_len = strlen(s1);
	s2_len = strlen(s2);
	result = malloc (s1_len + s2_len + 1);
	if (result != NULL)
	{
		i = 0;
		while (i < s1_len + s2_len)
		{
			if (i < s1_len)
				result[i] = s1[i];
			else
				result[i] = s2[i - s1_len];
			i++;
		}
		result[i] = '\0';
	}
	return (result);
}
