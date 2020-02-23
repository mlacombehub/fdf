/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlacombe <mlacombe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 00:22:31 by mlacombe          #+#    #+#             */
/*   Updated: 2020/02/23 17:26:00 by mlacombe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_vec3_t	fdf_projection(t_fdf_t *fdf, t_vec3_t point)
{
	t_vec3_t	quo;
	double		ray;
	double		bump;

	ray = (fdf->min == fdf->max ? 1 : (fdf->max - fdf->min) * 2);
	bump = (point.z + (fdf->max - fdf->min) / 4.) * fdf->scale.y;
	quo = (t_vec3_t) {point.x / (fdf->max_len - 1), point.y / fdf->nb_line, 0};
	if (fdf->projection == 1)
		point = (t_vec3_t){
			(bump + ray) * sin(quo.x * 2 * M_PI) * cos(quo.y * M_PI),
			(bump + ray) * sin(quo.y * M_PI),
			(bump + ray) * cos(quo.x * 2 * M_PI) * cos(quo.y * M_PI)
		};
	else if (fdf->projection == 2)
		point = (t_vec3_t){
			(bump + ray) * sin(quo.x * 2 * M_PI), point.y,
			(bump + ray) * cos(quo.x * 2 * M_PI)
		};
	return (point);
}

static t_vec3_t	fdf_transform(t_fdf_t *fdf, t_vec3_t point)
{
	t_vec3_t	ref_point;
	double		ref_z;
	double		persp;
	double		diam;

	ref_point = point;
	diam = (fdf->min == fdf->max ? 2 : abs(fdf->max - fdf->min) * 4) +
		hypot(hypot(fdf->max_len, fdf->nb_line), fdf->max - fdf->min);
	point = fdf_translate((t_vec3_t) {(fdf->max_len - 1) / -2.,
		(fdf->nb_line - 1) / -2., (fdf->max - fdf->min) / -4.}, point);
	ref_z = point.z;
	point.z *= fdf->scale.y;
	point = fdf_projection(fdf, point);
	point = fdf_rotation(fdf->rotation, point);
	persp = ref_z / diam + 1;
	point = fdf_scaling(fdf->scale, point);
	if (fdf->perspective & 1)
		point = (t_vec3_t) {point.x * persp, point.y * persp, point.z * persp};
	point = fdf_translate((t_vec3_t) {WIN_X / 2. + fdf->offset.x,
		WIN_Y / 2. + fdf->offset.y, fdf->offset.z}, point);
	point.z = 360 * (ref_point.z - fdf->min) / (fdf->max - fdf->min);
	return (point);
}

static void		fdf_put_pixel(t_fdf_t *fdf, t_vec3_t point, int color)
{
	uint32_t i;

	if (point.x < 0. || point.x >= WIN_X || point.y < 0. || point.y >= WIN_Y)
		return ;
	i = floor(point.x) + floor(point.y) * WIN_X;
	fdf->data_addr[i] = color;
}

static void		fdf_put_line(t_fdf_t *fdf, t_vec3_t a, t_vec3_t b, int color)
{
	t_vec3_t	d;
	double		max_d;

	d = (t_vec3_t) {b.x - a.x, b.y - a.y, b.z - a.z};
	max_d = fmax(fabs(d.x), fabs(d.y));
	d = (t_vec3_t) {d.x / max_d, d.y / max_d, d.z / max_d};
	while (max_d >= 0)
	{
		if (a.z == 0)
			color = 0x505050;
		else
			color = hsl2rgb(a.z, .5, .5);
		color &= 0xFFFFFF;
		fdf->projection >= 1 ? color |= 0x7F000000 : 0;
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
