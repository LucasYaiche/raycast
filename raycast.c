/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyaiche <lyaiche@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 16:00:08 by lyaiche           #+#    #+#             */
/*   Updated: 2022/06/06 17:46:00 by lyaiche          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycast.h"
	int map[] =
	{
		1,1,1,1,1,1,1,1,
		1,0,1,0,0,0,0,1,
		1,0,1,0,0,0,0,1,
		1,0,1,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,1,0,1,
		1,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,
	};

float degToRad(float a)
{ 
	return a*M_PI/180.0;
}

float FixAng(float a)
{
	if (a>359)
	{ 
		a-=360.0;
	}
	if(a<0)
	{ 
		a+=360.0;
	} 
	return a;
}

int	end(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	exit(0);
	return (0);
}



void	put_pixel(float x, float y, int color, t_data *data)
{
	int	i;

	i = ((int)x * data->bits_per_pixel / 8) + ((int)y * data->line_bytes);
	data->addr[i] = color;
	data->addr[++i] = color >> 8;
	data->addr[++i] = color >> 16;
}

void	draw_cube(float current_x, float current_y, int size, t_data *data, int color)
{
	int x,y;
	
	
	y = -1;
	while(++y < size)
	{
		x = -1;
		while(++x < size)
			put_pixel(current_x + x, current_y + y, color, data);
	}
	
}

void	draw_map(t_data *data)
{
	int		x,y;
	float	current_x, current_y;

	current_x = 0;
	current_y = 0;
	y = -1;
	while (++y < data->width)
	{
		x = -1;
		while(++x < data->length)
		{
			if (map[y*data->length + x] == 1)
			{
				draw_cube(current_x, current_y, data->tile_size, data, 0xffffff);
			}
			current_x += 100;
		}
		current_x = 0;
		current_y += 100;
	}
}

void	perspective(float *x, float *y, t_data *data)
{	
	int	temp_x;
	int	temp_y;

	temp_x = *x;
	temp_y = *y;
	*x = temp_x * cos(data->pa)
		- temp_y * sin(data->pa);
	*y = temp_x * sin(data->pa)
		+ temp_y * cos(data->pa);
}

void	draw_line(float current_x, float current_y, float next_x, float next_y, int color, t_data *data)
{
	float	dx;
	float	dy;
	int		max;

	dx = next_x - current_x;
	dy = next_y - current_y;
	max = fmax(fabs(dx), fabs(dy));
	dx /= max;
	dy /= max;
	while ((int)(current_x - next_x)
			|| (int)(current_y - next_y))
	{
		if (current_x < 1920 && current_y < 1080
			&& current_x > 0 && current_y > 0)
			put_pixel(current_x, current_y, color, data);
		current_x += dx;
		current_y += dy;
	}
}

void	vertline(int x, int draw_start, int draw_end, int color, t_data *data)
{
	int y,i;

	i = -1;
	y = draw_start;
	while(++i < draw_start)
		put_pixel(x, i, 0xBAE5F4, data);
	while  (y < draw_end)
	{
		put_pixel(x, y, color, data);
		y++;
	}
	while (y < 1080)
	{
		put_pixel(x, y, 0x5b5b5b, data);
		y++;
	}
}

void	draw3drays(t_data *data)
{
	int	r, mx, my, mp, dof, found, line_height, draw_start, draw_end, side, color, step_x, step_y;
	float  raymap_x, raymap_y, ray_step_x, ray_step_y, raystart_x, raystart_y, 
			lenght_x, lenght_y, distance, maxdistance, ra, dirx, diry, walldist, tick;
	
	ra = FixAng(data->pa + 30.0);
	tick =  60.0 / 1920.0;
	for(r=0; r<1919;r++)
	{
		color = 0x6a2633;
		dirx = cos(degToRad(ra));
		diry = -sin(degToRad(ra));
		ray_step_x = sqrtf(1.0 + ((diry / dirx) * (diry / dirx)));
		ray_step_y = sqrtf(1.0 + ((dirx / diry) * (dirx / diry)));
		raystart_x = data->px;
		raystart_y = data->py;
		raymap_x = (int)data->px;
		raymap_y = (int)data->py;
		if (dirx < 0)
		{
			step_x = -1;
			lenght_x = (raystart_x - raymap_x) * ray_step_x;
		}
		else
		{
			step_x = 1;
			lenght_x = ((raymap_x + 1.0) - raystart_x) * ray_step_x;
		}
		if (diry < 0)
		{
			step_y = -1;
			lenght_y = (raystart_y - raymap_y) * ray_step_y;
		}
		else
		{
			step_y = 1;
			lenght_y = ((raymap_y + 1.0) - raystart_y) * ray_step_y;
		}
		distance = 0;
		maxdistance = 8;
		found = 0;
		while(found==0 && distance <  maxdistance)
		{
			if (lenght_x < lenght_y)
			{
				raymap_x += step_x;
				lenght_x += ray_step_x;
				side = 0;
			}
			else
			{
				raymap_y += step_y;
				lenght_y += ray_step_y;
				side = 1;
			}
			if (map[(int)raymap_y * data->length + (int)raymap_x] == 1)
			{
				found = 1;
			}
		}
		// draw_line(data->px, data->py, raystart_x * data->tile_size + (dirx * distance * data->tile_size), raystart_y* data->tile_size + (diry * distance * data->tile_size), 0x3FFF00,data);
		if (side == 0)
			walldist = lenght_x - ray_step_x;
		else
			walldist = lenght_y - ray_step_y;
		walldist *= cos(degToRad(data->pa - ra));
		line_height = (int) (1080 / walldist);
		
		draw_start = -line_height / 2 + 1080 / 2;
		if (draw_start < 0)
			draw_start = 0;

		draw_end = line_height / 2 + 1080 / 2;
		if (draw_end >= 1080)
			draw_end = 1080 - 1;

		if (side == 1)
			color = 0xff0000 ;
		vertline(r, draw_start, draw_end, color, data);
		ra = FixAng(ra - tick);
	}
}

void	draw_player(t_data *data)
{
	draw_cube(data->px, data->py, 8, data, 0xffd400);
	draw_line(data->px, data->py, data->px+data->pdx*15, data->py+data->pdy*15,0xffffff, data);
}

int	launch(t_data *data)
{
	
	data->img = mlx_new_image(data->mlx, 1920, 1080);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_bytes, &data->endian);
	// draw_map(data);
	// draw_player(data);
	draw3drays(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}

int	key_hook(int keycode, t_data *data)
{
	float old_x, old_y;
	
	old_x = data->px;
	old_y = data->py;
	if (keycode == 53)
		end(data);
	if (keycode == 0)
		{ 
				data->px+=data->pdy * 0.4 ; data->py-=data->pdx * 0.4 ;
		}
	if (keycode == 2)
		{ 
				data->px-=data->pdy * 0.4 ; data->py+=data->pdx * 0.4 ;
		}
	if (keycode == 13)
		{ 
				data->px+=data->pdx * 0.4 ; data->py+=data->pdy * 0.4 ;
		}
	if (keycode == 1)
		{ 
				data->px-=data->pdx * 0.4 ; data->py-=data->pdy * 0.4 ;
		}
	if (keycode == 123)
	{
		data->pa+=4; 
		data->pa = FixAng(data->pa);
		data->pdx=cos(degToRad(data->pa));
		data->pdy=-sin(degToRad(data->pa));
	}
	if (keycode == 124)
	{ 
		data->pa -= 4; 
		data->pa = FixAng(data->pa);
		data->pdx = cos(degToRad(data->pa));
		data->pdy = -sin(degToRad(data->pa));
	}
	if (map[(int)data->py * data->length + (int)data->px] == 1)
	{
		data->px = old_x;
		data->py = old_y;
	}
	mlx_clear_window(data->mlx, data->win);
	launch(data);
	return (0);
}


int main(void)
{
	t_data *data;
	data = malloc(sizeof(t_data));
	data->mlx = mlx_init();
	data->width = 8;
	data->length = 8;
	data->tile_size = 100;
	data->px = 1.5;
	data->py = 1.5;
	data->pa = 90;
	data->pdx = cos(degToRad(data->pa));
	data->pdy = -sin(degToRad(data->pa));
	data->win = mlx_new_window(data->mlx, 1920, 1080, "raycast");
	launch(data);
	mlx_do_key_autorepeaton(data->mlx);
	mlx_hook(data->win, 2, 1L << 0, key_hook, data);
	mlx_hook(data->win, 17, 1L << 5, end, data);
	mlx_loop(data->mlx);
}


//-lmlx -framework OpenGL -framework AppKit