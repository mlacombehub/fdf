/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlacombe <mlacombe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 13:28:46 by mlacombe          #+#    #+#             */
/*   Updated: 2019/02/26 19:25:15 by mlacombe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrev(char *str)
{
	int		i;
	int		j;
	char	t;

	i = 0;
	j = 0;
	while (str[j])
		j++;
	while (i < j)
	{
		t = str[j];
		str[j++] = str[i];
		str[i++] = t;
	}
	return (str);
}
