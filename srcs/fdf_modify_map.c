/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_modify_map.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlacombe <mlacombe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 14:56:15 by mlacombe          #+#    #+#             */
/*   Updated: 2020/02/14 16:14:50 by mlacombe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_vec3_t		fdf_translate(t_vec3_t offset, t_vec3_t point)
{
	point.x += offset.x;
	point.y += offset.y;
	point.z += offset.z;
	return (point);
}

t_vec3_t		fdf_scaling(t_vec2_t scale, t_vec3_t point)
{
	point.x *= scale.x;
	point.y *= scale.x;
	point.z *= scale.x * scale.y;
	return (point);
}

t_vec3_t		fdf_rotation(t_vec3_t rot, t_vec3_t point, t_fdf_t *fdf)
{
	t_vec3_t c;
	t_vec3_t s;
	t_vec3_t ref;

	c = (t_vec3_t){fdf_pcos(rot.x, 5), fdf_pcos(rot.y, 5), fdf_pcos(rot.z, 5)};
	s = (t_vec3_t){fdf_psin(rot.x, 5), fdf_psin(rot.y, 5), fdf_psin(rot.z, 5)};
	fdf->mtrx.a1 = c.y * c.z;
	fdf->mtrx.a2 = -c.y * s.z;
	fdf->mtrx.a3 = s.y;
	fdf->mtrx.b1 = s.x * s.y * c.z + c.x * s.z;
	fdf->mtrx.b2 = c.x * c.z - s.x * s.y * s.z;
	fdf->mtrx.b3 = -s.x * c.y;
	fdf->mtrx.c1 = s.x * s.z - c.x * s.y * c.z;
	fdf->mtrx.c2 = c.x * s.y * s.z + s.x * c.z;
	fdf->mtrx.c3 = c.x * c.y;
	ref = point;
	point.x = ref.x * fdf->mtrx.a1 + ref.y * fdf->mtrx.a2 + ref.z * fdf->mtrx.a3;
	point.y = ref.x * fdf->mtrx.b1 + ref.y * fdf->mtrx.b2 + ref.z * fdf->mtrx.b3;
	point.z = ref.x * fdf->mtrx.c1 + ref.y * fdf->mtrx.c2 + ref.z * fdf->mtrx.c3;
	return (point);
}

static double hue2rgb(double v1, double v2, double vh)
{
	if (vh < 0)
		vh += 1;
	if (vh > 1)
		vh -= 1;
	if ((6 * vh) < 1)
		return (v1 + (v2 - v1) * 6 * vh);
	if ((2 * vh) < 1)
		return (v2);
	if ((3 * vh) < 2)
		return (v1 + (v2 - v1) * ((2.0f / 3) - vh) * 6);
	return (v1);
}

int 			hsl2rgb(int hue, double saturation, double luminance)
{
	int rgb;
	double v1;
	double v2;
	double angle;

	if (saturation == 0)
	{
		rgb = (char)(luminance * 255);
		rgb |= (rgb << 16) | (rgb << 8);
	}
	else
	{
		angle = (double)hue / 360;
		if (luminance < 0.5)
			v2 = luminance * (1 + saturation);
		else
			v2 = (luminance + saturation) - (luminance * saturation);
		v1 = 2 * luminance - v2;
		rgb = (((char)(255. * hue2rgb(v1, v2, angle + (1.0f / 3)))) << 16)
			| (((int)(255. * hue2rgb(v1, v2, angle))) << 8)
			| ((int)(255. * hue2rgb(v1, v2, angle - (1.0f / 3))));
	}
	return (rgb);
}
