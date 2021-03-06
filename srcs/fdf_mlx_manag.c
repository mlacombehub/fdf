/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_mlx_manag.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlacombe <mlacombe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 00:22:31 by mlacombe          #+#    #+#             */
/*   Updated: 2020/02/21 18:25:12 by mlacombe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	close_screen(void *param)
{
	(void)param;
	exit(1);
	return (0);
}

void		fdf_mlx_crea(t_fdf_t *fdf)
{
	if (!(fdf->mlx = mlx_init()))
		close_screen((void *)0);
	if (!(fdf->img = mlx_new_image(fdf->mlx, WIN_X, WIN_Y)))
	{
		ft_memdel(fdf->mlx);
		close_screen((void *)0);
	}
	if (!(fdf->win = mlx_new_window(fdf->mlx, WIN_X, WIN_Y, fdf->progname)))
	{
		mlx_destroy_image(fdf->mlx, fdf->img);
		ft_memdel(fdf->mlx);
		close_screen((void *)0);
	}
	fdf->data_addr = (int *)mlx_get_data_addr(fdf->img, &fdf->bits_per_pixel,
		&fdf->line_size, &fdf->endian);
}

static void	fdf_hooker_texas_ranger2(int key, t_fdf_t *fdf)
{
	key == W ? fdf->rotation.x += M_PI / 36 : 42;
	key == S ? fdf->rotation.x -= M_PI / 36 : 42;
	key == A ? fdf->rotation.y -= M_PI / 36 : 42;
	key == D ? fdf->rotation.y += M_PI / 36 : 42;
	key == Q ? fdf->rotation.z -= M_PI / 36 : 42;
	key == E ? fdf->rotation.z += M_PI / 36 : 42;
	if (key == ENTER)
	{
		fdf->projection = 0;
		fdf->offset = (t_vec3_t) {0};
		fdf->scale = fdf->o_scale;
		fdf->rotation = (t_vec3_t) {0};
	}
	if (key == I)
	{
		fdf->projection = 0;
		fdf->rotation = (t_vec3_t) {M_PI / 3, 0, M_PI / 4};
	}
	key == P ? fdf->perspective ^= 1 : 42;
	key == J ? fdf->projection = 0 : 42;
	key == K ? fdf->projection = 1 : 42;
	key == L ? fdf->projection = 2 : 42;
}

static int	fdf_hooker_texas_ranger(int key, t_fdf_t *fdf)
{
	key == ESC ? close_screen(NULL) : 42;
	key == LEFT ? fdf->offset.x -= fdf->o_scale.x : 42;
	key == RIGHT ? fdf->offset.x += fdf->o_scale.x : 42;
	key == DOWN ? fdf->offset.y -= fdf->o_scale.x : 42;
	key == UP ? fdf->offset.y += fdf->o_scale.x : 42;
	key == PLUS ? fdf->scale.x *= 1.1 : 42;
	key == MINUS ? fdf->scale.x /= 1.1 : 42;
	key == X ? fdf->scale.y *= 1.1 : 42;
	key == Z ? fdf->scale.y /= 1.1 : 42;
	fdf_hooker_texas_ranger2(key, fdf);
	fdf_mlx_data_addressor(fdf);
	return (0);
}

void		fdf_mlx_manag(t_fdf_t *fdf)
{
	fdf->o_scale = (t_vec2_t) {fmin(WIN_X / (fdf->max_len + 1),
		WIN_Y / (fdf->nb_line + 1)), 1};
	fdf->scale = fdf->o_scale;
	fdf->perspective = 0;
	fdf->projection = 0;
	fdf->rotation = (t_vec3_t) {M_PI / -9, M_PI / -9, 0};
	fdf_mlx_data_addressor(fdf);
	mlx_hook(fdf->win, 17, 1, close_screen, fdf);
	mlx_hook(fdf->win, 2, 1, fdf_hooker_texas_ranger, fdf);
	mlx_loop(fdf->mlx);
}
