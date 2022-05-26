/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyaiche <lyaiche@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 16:00:08 by lyaiche           #+#    #+#             */
/*   Updated: 2022/05/26 15:50:07 by lyaiche          ###   ########.fr       */
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

float degToRad(int a)
{ 
	return a*M_PI/180.0;
}

int FixAng(int a)
{
	if(a>359)
	{ 
		a-=360;
	}
	if(a<0)
	{ 
		a+=360;
	} 
	return a;
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
	int x,y;
	float current_x, current_y;

	current_x = 0;
	current_y = 0;
	y = -1;
	while(++y < data->width)
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

void	draw3drays(t_data *data)
{
	// int	r, mx, my, mp, dof;
	// float rx, ry, ra, x, y;

	// ra=data->pa; 
	// for(r=0; r<1;r++)
	// {
	// 	dof=0;
	// 	float atan = -1/tan(ra);
	// 	atan = 1/atan;
	// 	if (ra > M_PI)
	// 	{
	// 		ry=(((int)data->py>>6)<<6)-0.0001;
	// 		rx=(data->py-ry)*atan+data->px;
	// 		y=-64;
	// 		x=-y*atan;
	// 	}
	// 	if (ra < M_PI)
	// 	{
	// 		ry=(((int)data->py>>6)<<6)+64;
	// 		rx=(data->py-ry)*atan+data->px;
	// 		y=64;
	// 		x=-y*atan;
	// 	}
	// 	if (ra==0 || ra==M_PI)
	// 	{
	// 		rx=data->px;
	// 		ry=data->py;
	// 		dof=8;
	// 	}
	// 	while(dof<8)
	// 	{
	// 		mx=(int)(rx)>>6;
	// 		my=(int)(ry)>>6;
	// 		mp=my*data->length+mx;
	// 		if(mp<data->length*data->width && map[mp]==1)
	// 		{
	// 			dof=8;
	// 		}
	// 		else
	// 		{
	// 			rx+=x;
	// 			ry+=y;
	// 			dof+=1;
	// 		}
	// 	}
	// 	draw_line(data->px, data->py, rx, ry, 0x3FFF00,data);
	// }
	int	r, mx, my, mp, dof, found;
	float step_x, step_y, raymap_x, raymap_y, ray_step_x, ray_step_y, raystart_x, raystart_y, lenght_x, lenght_y, distance, maxdistance;

	ray_step_x = sqrt(1 + ((data->pdy / data->pdx) * (data->pdy / data->pdx)));
	ray_step_y = sqrt(1 + ((data->pdx / data->pdy) * (data->pdx / data->pdy)));
	raystart_x = data->tile_x;
	raystart_y = data->tile_y;
	raymap_x = (int)data->tile_x;
	raymap_y = (int)data->tile_y;
	
	for(r=0; r<1;r++)
	{
		if (data->pdx < 0)
		{
			step_x = -1;
			lenght_x = (raystart_x - raymap_x) * ray_step_x;
		}
		else
		{
			step_x = 1;
			lenght_x = ((raymap_x + 1) - raystart_x) * ray_step_x;
		}
		if (data->pdy < 0)
		{
			step_y = -1;
			lenght_y = (raystart_y - raymap_y) * ray_step_y;
		}
		else
		{
			step_y = 1;
			lenght_y = ((raymap_y + 1) - raystart_y) * ray_step_y;
		}
		distance = 0;
		maxdistance = 8;
		found = 0;
			printf("%f\n%f\n", raymap_x, raymap_y);
		while(found==0 && distance <  maxdistance)
		{
			if (lenght_x < lenght_y)
			{
				raymap_x += step_x;
				distance = lenght_x;
				lenght_x += ray_step_x;
			}
			else
			{
				raymap_y += step_y;
				distance = lenght_y;
				lenght_y += ray_step_y;
			}
			if (raymap_x >= 0 && raymap_x < data->length && raymap_y >= 0 && raymap_y < data->width)
			{
				if (map[(int)raymap_y * data->length + (int)raymap_x] == 1)
				{
					printf("salut\n");
					found = 1;
				}
			}
		}
		// printf("%d\n", found);
		printf("%f\n%f\n", raystart_x * data->tile_size + data->pdx*distance, distance);
		draw_line(data->px, data->py, raystart_x * data->tile_size + (data->pdx * distance * data->tile_size), raystart_y* data->tile_size + (data->pdy * distance * data->tile_size), 0x3FFF00,data);
	}
}

void	draw_player(t_data *data)
{
	draw_cube(data->px, data->py, 8, data, 0xffd400);
	draw_line(data->px, data->py, data->px+data->pdx*15, data->py+data->pdy*15,0xffffff, data);
}

void	launch(t_data *data)
{
	
	data->img = mlx_new_image(data->mlx, 1920, 1080);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_bytes, &data->endian);
	draw_map(data);
	draw_player(data);
	draw3drays(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

int	end(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	exit(0);
	return (0);
}

void tile(t_data *data)
{
	data->tile_x = data->px / data->tile_size;
	data->tile_y = data->py / data->tile_size;
}

int	key_hook(int keycode, t_data *data)
{
	if (keycode == 53)
		end(data);
	if (keycode == 0)
		{ data->px+=data->pdy; data->px+=data->pdx;}
	if (keycode == 2)
		{ data->px-=data->pdy; data->px-=data->pdx;}
	if (keycode == 13)
		{ data->px+=data->pdx; data->py+=data->pdy;}
	if (keycode == 1)
		{ data->px-=data->pdx; data->py-=data->pdy;}
	if (keycode == 123)
	{
		data->pa+=5; 
		data->pa = FixAng(data->pa);
		data->pdx=cos(degToRad(data->pa));
		data->pdy=-sin(degToRad(data->pa));
	}
	if (keycode == 124)
	{ 
		data->pa -= 5; 
		data->pa = FixAng(data->pa);
		data->pdx = cos(degToRad(data->pa));
		data->pdy = -sin(degToRad(data->pa));
	}
	tile(data);
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
	data->px = 120;
	data->py = 120;
	data->pa = 90;
	data->pdx = cos(degToRad(data->pa));
	data->pdy = -sin(degToRad(data->pa));
	tile(data);
	data->win = mlx_new_window(data->mlx, 1920, 1080, "raycast");
	launch(data);
	mlx_do_key_autorepeaton(data->mlx);
	mlx_hook(data->win, 2, 1L << 0, key_hook, data);
	mlx_hook(data->win, 17, 1L << 5, end, data);
	mlx_loop(data->mlx);
}


//-lmlx -framework OpenGL -framework AppKit