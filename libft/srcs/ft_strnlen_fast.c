/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_fast.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlacombe <mlacombe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 16:35:46 by mlacombe          #+#    #+#             */
/*   Updated: 2019/05/15 14:04:47 by mlacombe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

uint8_t		ft_strlen_fast(const char *str)
{
	uint8_t		len;
	uint32_t	*ref;

	len = 0;
	ref = (uint32_t *)str;
	while (1)
	{
		if (!(*ref & 0x000000FF))
			return (len);
		else if (!(*ref & 0x0000FF00))
			return (len + 1);
		else if (!(*ref & 0x00FF0000))
			return (len + 2);
		else if (!(*ref & 0xFF000000))
			return (len + 3);
		len += 4;
		ref++;
	}
	return (0);
}
