/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlacombe <mlacombe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 00:22:31 by mlacombe          #+#    #+#             */
/*   Updated: 2020/02/18 17:01:54 by mlacombe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_vec3_t	fdf_transform(t_fdf_t *fdf, t_vec3_t point)
{
    t_vec3_t	ref_point;
	double quo;
	double ray;

    ref_point = point;
	point = fdf_translate((t_vec3_t) {(fdf->max_len - 1) / -2.,
		(fdf->nb_line - 1) / -2., (fdf->max + fdf->min) / -2.},point);
	point = fdf_scaling(fdf->scale, point);
	point = fdf_rotation(fdf->rotation, point, fdf);
	if (fdf->perspective == 1)
	{
		ray = (fdf->scale.x) * (fdf->scale.y);
		quo = sqrtf(powf(point.x, 2) + powf(point.y, 2) + powf(point.z, 2));
		point.x = ray * point.x / sqrtf(powf(ray, 2) - 2 * ray * point.z + powf(quo, 2));
		point.y = ray * point.y / sqrtf(powf(ray, 2) - 2 * ray * point.z + powf(quo, 2));
	}
	point = fdf_translate((t_vec3_t){WIN_X / 2. + fdf->offset.x,
		WIN_Y / 2. + fdf->offset.y, fdf->offset.z}, point);
	point.z = ref_point.z;
	return (point);
}

static void		fdf_put_pixel(t_fdf_t *fdf, t_vec3_t point, int color)
{
	int i;

	if (point.x < 0. || point.x >= WIN_X || point.y < 0. || point.y >= WIN_Y)
		return;
	i = floor(point.x) + floor(point.y) * WIN_X;
	fdf->data_addr[i] = color;
}

static void		fdf_put_line(t_fdf_t *fdf, t_vec3_t a, t_vec3_t b, int color)
{
	t_vec3_t d;
	double max_d;

	d = (t_vec3_t) {b.x - a.x, b.y - a.y, b.z - a.z};
	max_d = fmax(fabs(d.x), fabs(d.y));
	d = (t_vec3_t) {d.x / max_d, d.y / max_d, d.z / max_d};
	while (max_d >= 0)
	{
		if ((fdf->min || fdf->max) && fdf->max + fdf->min != 0)
			color = hsl2rgb((a.z + fdf->min)
				/ (fdf->max + fdf->min) * 360, .5, .5);
		else
			color = hsl2rgb((a.z) * 360, .5, .5);
		color &= 0xFFFFFF;
        fdf_put_pixel(fdf, a, color);
		a = (t_vec3_t){a.x + d.x, a.y + d.y, a.z + d.z};
		max_d--;
	}
}

void			fdf_mlx_data_addressor(t_fdf_t *fdf)
{
	t_point_t	i;
	t_vec3_t	o_point;

	ft_bzero(fdf->data_addr, fdf->line_size * WIN_Y);
	i.y = -1;
	while (++i.y < fdf->nb_line)
	{
		i.x = -1;
		while (++i.x < fdf->line_len[i.y])
		{
			o_point = fdf_transform(fdf, fdf->points[i.y][i.x]);
			if (i.y + 1 < fdf->nb_line && i.x < fdf->line_len[i.y + 1])
				fdf_put_line(fdf, o_point,
					fdf_transform(fdf, fdf->points[i.y + 1][i.x]), 0xFFFFFF);
			if (i.x + 1 < fdf->line_len[i.y])
				fdf_put_line(fdf, o_point,
					fdf_transform(fdf, fdf->points[i.y][i.x + 1]), 0xFFFFFF);
		}
	}
	if (i.x == 1 & i.y == 2)
		fdf_put_pixel(fdf, o_point, 0xFFFFFF);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->img, 0, 0);
}
