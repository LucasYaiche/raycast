/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyaiche <lyaiche@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 16:00:08 by lyaiche           #+#    #+#             */
/*   Updated: 2022/06/09 17:06:54 by lyaiche          ###   ########.fr       */
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



void	put_pixel(int x, int y, int color, t_data *data)
{
	char	*pxl;

	pxl = data->addr + (x * data->bits_per_pixel / 8) + (y * data->line_bytes);
	*(unsigned int *)pxl = color;
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


int getpix(int x, int y, t_img *img)
{
	char	*pxl;

	pxl = img->addr + (x * img->bpp / 8) + (y * img->lb);
	return (*(int *)pxl);
}

void	vertline(int x, int side, t_data *data, t_img *img)
{
	int i,w,h, color;
	float y;
	i = -1;

	y = data->draw_start;
	h = 1080;
	w = 1920;
	while  (data->draw_start < data->draw_end)
	{	
		if (data->draw_start>= 0 && data->draw_start <= 1079)
		{
			if (side == 0)
				color = getpix((data->intery - (int)(data->intery)) * (float)w, ((float)data->draw_start - y) / (float)(data->draw_end - y) * (float)h,img);
			else
				color = getpix((data->interx - (int)(data->interx)) * (float)w, ((float)data->draw_start - y) / (float)(data->draw_end - y) * (float)h,img);
			put_pixel(x, data->draw_start, color, data);
		}
		data->draw_start++;
	}
}

void	draw3drays(t_data *data)
{
	int	r, mx, my, mp, dof, found, line_height, line_width,  side, color, step_x, step_y, i;
	float  raymap_x, raymap_y, ray_step_x, ray_step_y, raystart_x, raystart_y, 
			lenght_x, lenght_y, distance, maxdistance, ra, dirx, diry, walldist, tick, draw_start, draw_end;
	void *image;
	
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
		walldist = 0;
		maxdistance = 8;
		found = 0;
		while(found==0 && walldist <  maxdistance)
		{
			if (lenght_x < lenght_y)
			{
				raymap_x += step_x;
				walldist = lenght_x;
				lenght_x += ray_step_x;
				side = 0;
			}
			else
			{
				raymap_y += step_y;
				walldist = lenght_y;
				lenght_y += ray_step_y;
				side = 1;
			}
			if (map[(int)raymap_y * data->length + (int)raymap_x] == 1)
			{
				found = 1;
				data->interx = raystart_x + (dirx * walldist);
				data->intery = raystart_y + (diry * walldist);
				data->raylen = sqrtf(powf(data->interx - data->px, 2) + powf(data->intery - data->py, 2));
			}
		}
		// draw_line(data->px, data->py, raystart_x * data->tile_size + (dirx * distance * data->tile_size), raystart_y* data->tile_size + (diry * distance * data->tile_size), 0x3FFF00,data);
		data->draw_start = 540.0 - (1080.0 / (data->raylen * (cos(degToRad(data->pa - ra)))) / 2.0);
		data->draw_end = 540.0 + (1080.0 / (data->raylen * (cos(degToRad(data->pa - ra)))) / 2.0);
		i = -1;
		if (draw_start >= 0)
		{
			while(++i < data->draw_start)
				put_pixel(r, i, 0xBAE5F4, data);
			i = data->draw_end;
			while(i < 1080)
				put_pixel(r, i++, 0x5b5b5b, data);
		}
		if (side && cos(degToRad(ra + 90.0)) > 0.0)
		{
			// printf("1\n");
			vertline(r, side, data, &data->west);
		}
		else if (side)
		{
			// printf("2\n");
			vertline(r, side ,data, &data->east);
		}
		else if (sin(degToRad(ra + 90.0)) > 0.0)
		{
			// printf("3\n");
			vertline(r, side, data, &data->south);
		}
		else
		{
			// printf("4\n");
			vertline(r, side, data, &data->north);
		}
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
	int width, height;

	data = malloc(sizeof(t_data));
	data->mlx = mlx_init();
	data->width = 8;
	data->length = 8;
	data->tile_size = 100;
	data->px = 1.5;
	data->py = 1.5;
	data->pa = 90;
	width = 1920;
	height = 1080;
	data->pdx = cos(degToRad(data->pa));
	data->pdy = -sin(degToRad(data->pa));
	data->north.img = mlx_xpm_file_to_image(data->mlx, "forest.xpm", &height, &width);
	data->north.addr = mlx_get_data_addr(data->north.img, &data->north.bpp, &data->north.lb, &data->north.endian);
	data->south.img = mlx_xpm_file_to_image(data->mlx, "pixar.xpm", &height, &width);
	data->south.addr = mlx_get_data_addr(data->south.img, &data->south.bpp, &data->south.lb, &data->south.endian);
	data->west.img = mlx_xpm_file_to_image(data->mlx, "disney.xpm", &height, &width);
	data->west.addr = mlx_get_data_addr(data->west.img, &data->west.bpp, &data->west.lb, &data->west.endian);
	data->east.img = mlx_xpm_file_to_image(data->mlx, "dreamworks.xpm", &height, &width);
	data->east.addr = mlx_get_data_addr(data->east.img, &data->east.bpp, &data->east.lb, &data->east.endian);
	data->win = mlx_new_window(data->mlx, 1920, 1080, "raycast");
	launch(data);
	mlx_do_key_autorepeaton(data->mlx);
	mlx_hook(data->win, 2, 1L << 0, key_hook, data);
	mlx_hook(data->win, 17, 1L << 5, end, data);
	mlx_loop(data->mlx);
}


//-lmlx -framework OpenGL -framework AppKit