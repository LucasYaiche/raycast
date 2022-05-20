/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyaiche <lyaiche@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 16:00:08 by lyaiche           #+#    #+#             */
/*   Updated: 2022/05/20 18:09:21 by lyaiche          ###   ########.fr       */
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

void	put_pixel(float x, float y, int color, t_data *data)
{
	int	i;

	i = (x * data->bits_per_pixel / 8) + (y * data->line_bytes);
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
				draw_cube(current_x, current_y, 100, data, 0xffffff);
			}
			current_x += 100;
		}
		current_x = 0;
		current_y += 100;
	}
}

void	perspective(float *x, float *y, t_data *data)
{	
	float	temp_x;
	float	temp_y;

	temp_x = *x;
	temp_y = *y;
	*x = temp_x * cos(data->angle)
		- temp_y * sin(data->angle);
	*y = temp_x * sin(data->angle)
		+ temp_y * cos(data->angle);
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

void	draw_player(t_data *data)
{
	data->current_x = data->pos_x + 5;
	data->current_y = data->pos_y + 5;
	perspective(&data->next_x, &data->next_y, data);
	// perspective(&data->current_x, &data->current_y, data);
	// printf("%f\n", data->angle);
	draw_cube(data->pos_x, data->pos_y, 10, data, 0xffd400);
	draw_line(data->current_x, data->current_y, data->next_x, data->next_y, 0xffffff, data);
}

void	launch(t_data *data)
{
	data->img = mlx_new_image(data->mlx, 1920, 1080);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_bytes, &data->endian);
	draw_player(data);
	draw_map(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

int	end(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	exit(0);
	return (0);
}

int	key_hook(int keycode, t_data *data)
{
	if (keycode == 53)
		end(data);
	if (keycode == 2)
		if (data->pos_x < 10000)
			data->pos_x += 10;
	if (keycode == 0)
		if (data->pos_x > -10000)
			data->pos_x -= 10;
	if (keycode == 1)
		if (data->pos_y < 10000)
			data->pos_y += 10;
	if (keycode == 13)
		if (data->pos_y > -10000)
			data->pos_y -= 10;
	if (keycode == 123)
	{
		data->angle -= 1; 
		if (data->angle < 0)
			data->angle = 360;
	}
	if (keycode == 124)
	{
		data->angle += 1;
		if (data->angle > 360)
			data->angle = 0;
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
	data->pos_x = 120;
	data->pos_y =120;
	data->angle = 90;
	data->color_angle = 0xffd400;
	data->current_x = data->pos_x + 5;
	data->current_y = data->pos_y + 5;
	data->next_x = data->current_x;
	data->next_y = data->current_y - 15;
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, 1920, 1080, "raycast");
	launch(data);
	mlx_do_key_autorepeaton(data->mlx);
	mlx_hook(data->win, 2, 1L << 0, key_hook, data);
	mlx_hook(data->win, 17, 1L << 5, end, data);
	mlx_loop(data->mlx);
}


//-lmlx -framework OpenGL -framework AppKit