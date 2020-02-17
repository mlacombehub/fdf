/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_map_manag.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlacombe <mlacombe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 00:05:19 by mlacombe          #+#    #+#             */
/*   Updated: 2020/02/13 17:50:22 by mlacombe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void 	fdf_countlines(t_fdf_t *fdf, char *str)
{
	fdf->nb_line = 0;
	while (*str)
	{
		if (*str == '\n' && *str)
			fdf->nb_line++;
		str++;
	}
}

void	fdf_countcolumns(t_fdf_t *fdf, char *str)
{
	int i;

	i = 0;
	fdf->max_len = 0;
	if (!(fdf->line_len = (int *)malloc(sizeof(int) * fdf->nb_line)))
		return ((void)0);
	fdf->line_len[i] = 0;
	while (*str)
	{
		if (*str != ' ' && *str != '\n')
		{
			fdf->line_len[i]++;
			str++;
		}
		while (*str != ' ' && *str != '\n' && *str)
			str++;
		if (*str == '\n' || *(str + 1) == '\0')
		{
			fdf->max_len = ft_max(fdf->line_len[i], fdf->max_len);
			fdf->line_len[++i] = 0;
		}
		str++;
	}
}

double	fdf_pcos(double number, int precision)
{
	number = cos(number) * ft_power(10, precision);
	number = round(number) / ft_power(10, precision);
	return (number);
}

double	fdf_psin(double number, int precision)
{
	number = sin(number) * ft_power(10, precision);;
	number = round(number) / ft_power(10, precision);;
	return (number);
}
