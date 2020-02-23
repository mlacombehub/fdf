/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlacombe <mlacombe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 15:16:58 by mlacombe          #+#    #+#             */
/*   Updated: 2020/02/21 15:53:55 by mlacombe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "mlx.h"
# include "libft.h"
# include "keymap.h"
# include <math.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

/*
** WIN_X and WIN_Y are double
*/
# define WIN_X		1620.
# define WIN_Y		1000.
# define SPACE_X	50
# define SPACE_Y	50

/*
** Colors def
*/
# define C_MENU1	0x42FFFF
# define C_MENU2	0xFF42FF
# define C_MENU3	0xFFFF42
# define C_BORDER	0xFFFFFF
# define C_BACK		0x222222

# define BUFF_FDF	4095

/*
**	Transform scale
*/
# define SCA_BLOCK	20

typedef struct	s_vec2
{
	double		x;
	double		y;
}				t_vec2_t;

typedef struct	s_vec3
{
	double		x;
	double		y;
	double		z;
}				t_vec3_t;

typedef struct	s_matrix
{
	double		a1;
	double		a2;
	double		a3;
	double		b1;
	double		b2;
	double		b3;
	double		c1;
	double		c2;
	double		c3;
}				t_mtrx_t;

typedef struct	s_fdf
{
	void		*mlx;
	void		*win;
	void		*img;
	t_vec3_t	**points;
	t_vec3_t	offset;
	t_vec3_t	rotation;
	t_vec2_t	o_scale;
	t_vec2_t	scale;
	char		*progname;
	int32_t		*line_len;
	int32_t		*data_addr;
	int32_t		min;
	int32_t		max;
	uint32_t	max_len;
	int32_t		nb_line;
	int32_t		bits_per_pixel;
	int32_t		line_size;
	int32_t		endian;
	uint32_t	perspective;
	uint32_t	projection;
}				t_fdf_t;

void			fdf_countlines(t_fdf_t *fdf, char *str);
void 			fdf_countcolumns(t_fdf_t *fdf, char *str);

void			fdf_mlx_crea(t_fdf_t *fdf);

void			fdf_mlx_manag(t_fdf_t *fdf);
void			fdf_mlx_data_addressor(t_fdf_t *fdf);
t_vec3_t       	fdf_translate(t_vec3_t offset, t_vec3_t point);
t_vec3_t     	fdf_scaling(t_vec2_t scale, t_vec3_t ref);
t_vec3_t     	fdf_rotation(t_vec3_t rotation, t_vec3_t ref);
double			fdf_pcos(double number, int precision);
double			fdf_psin(double number, int precision);

int      		hsl2rgb(int hue, double saturation, double luminance);

#endif
