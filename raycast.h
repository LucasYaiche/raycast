#ifndef RAYCAST_H
#define RAYCAST_H

#include <math.h>
#include <mlx.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#define DR 0.0174533
#define W 1920
#define H 1080

typedef struct s_img {
	void	*img;
	char	*addr;
	int		w;
	int		h;
	int		bpp;
	int		lb;
	int		endian;
	struct s_data	*data;
}				t_img;

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
	float	interx;
	float	intery;
	float	current_x;
	float	current_y;
	float	next_x;
	float	next_y;
	float	tile_x;
	float	tile_y;
	int		step_x;
	int		step_y;
	float 	raylen;
	float	draw_start;
	float	draw_end;
	int		size_c;
	int		color_angle;
	int		line_height;
	int		line_width;
	char		col_r;
	char		col_g;
	char		col_b;
	t_img	north;
	t_img	south;
	t_img	west;
	t_img	east;
}				t_data;

#endif

// 