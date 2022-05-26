#ifndef RAYCAST_H
#define RAYCAST_H

#include <math.h>
#include <mlx.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>


typedef struct s_data{

	int		**map;
	int		length;
	int		width;
	int		tile_size;
	char	*name;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_bytes;
	int		endian;
	void	*mlx;
	void	*win;
	float	px;
	float	py;
	float	pa;
	float	pdx;
	float	pdy;
	float	current_x;
	float	current_y;
	float	next_x;
	float	next_y;
	float	tile_x;
	float	tile_y;
	int		step_x;
	int		step_y;
	int		size_c;
	int		color_angle;
}				t_data;

#endif