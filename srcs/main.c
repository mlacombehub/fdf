/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlacombe <mlacombe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 15:14:55 by mlacombe          #+#    #+#             */
/*   Updated: 2020/02/13 17:50:03 by mlacombe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static char	*fdf_reallocfile(int fd)
{
	long long	n;
	char 		*buff;
	char	 	*tmp;
	char	 	*result;
	int 		readres;

	if (!(buff = (char *)malloc(BUFF_FDF + 1)))
		ft_free_return(buff);
	n = 0;
	result = 0;
	while ((readres = read(fd, buff, BUFF_FDF)) > 0)
	{
		if (!(tmp = (char *)malloc(sizeof(char) * (n + readres))))
			ft_free_return(tmp);
		ft_memmove(tmp, result, n);
		ft_memmove(tmp + n, buff, readres);
		n += readres;
		free(result);
		result = tmp;
	}
	result[n] = 0;
	free(buff);
    return(result);
}

static char *manage_file(int ac, char **av)
{
	int		fd;
	char	*result;

	if (ac != 2)
	{
		perror("usage : ./fdf [input_file]");
		exit(-1);
	}
	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		perror("Problem when opening file");
		exit(-1);
	}
	if (read(fd, NULL, 0))
	{
		perror("Problem with reading file");
		exit(1);
	}
	result = fdf_reallocfile(fd);
	if (close(fd) < 0)
	{
		perror("Problem with closing file");
		exit(1);
	}
	return (result);
}

static void fdf_allocpoints(t_fdf_t *fdf, char *str)
{
	t_point_t i;

	i = (t_point_t) {-1, -1};
	if (!(fdf->points = (t_vec3_t **)malloc(
		sizeof(*fdf->points)* fdf->nb_line)))
		return ((void)0);
	while (++i.y < fdf->nb_line)
	{
		while (*str && *str == ' ')
			str++;
		if (!(fdf->points[i.y] = (t_vec3_t *)malloc(
			sizeof(*fdf->points[i.y])* fdf->line_len[i.y])))
			return ((void)0);
		while (++i.x < fdf->line_len[i.y] && *str != '\n')
		{
			fdf->points[i.y][i.x] = (t_vec3_t) {i.x, i.y, ft_atoi(str)};
			fdf->min = ft_min(fdf->points[i.y][i.x].z, fdf->min);
			fdf->max = ft_max(fdf->points[i.y][i.x].z, fdf->max);
			while (*str && *str != ' ' && *str != '\n')
				str++;
			if (*str == ' ')
				while (*str && *str == ' ' && *str != '\n')
					str++;
		}
		if (*str++)
			i.x = -1;
	}
}

int			main(int ac, char **av)
{
	char *raw_str;
	t_fdf_t fdf;

	ft_bzero(&fdf, sizeof(t_fdf_t *));
	fdf.progname = av[0];
	raw_str = manage_file(ac, av);
	fdf_countlines(&fdf, raw_str);
	fdf_countcolumns(&fdf, raw_str);
	fdf_allocpoints(&fdf, raw_str);
	fdf_mlx_crea(&fdf);
	fdf_mlx_manag(&fdf);
	return (0);
}
