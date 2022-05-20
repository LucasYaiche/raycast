#ifndef RAYCAST_H
#define RAYCAST_H

#include <math.h>
#include <mlx.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#define PI 3,1415926535

typedef struct s_data{

	int		**map;
	int		length;
	int		width;
	char	*name;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_bytes;
	int		endian;
	void	*mlx;
	void	*win;
	float	pos_x;
	float	pos_y;
	float	current_x;
	float	current_y;
	float	next_x;
	float	next_y;
	float	angle;
	int		size_c;
	int		color_angle;
}				t_data;

#endif