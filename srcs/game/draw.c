#include "cub3d.h"
#include "defines.h"

void put_pixel(t_vector pixel, t_img img, int color)
{
	char *img_pixel;
	int	  offset;

	if (pixel.x < 0 || pixel.y < 0)
		return;
	if (pixel.x >= img.width || pixel.y >= img.height)
		return;
	offset = (pixel.y * img.line_length + pixel.x * (img.bits_per_pixel / 8));
	img_pixel = img.addr + offset;
	*(unsigned int *)img_pixel = color;
}

void put_minimap_pixel(t_vector pixel, int color, t_data *data)
{
	pixel.x = pixel.x - (data->player.pos.x * data->settings.minimap_block_size) + (data->settings.minimap_center.x);
	pixel.y = pixel.y - (data->player.pos.y * data->settings.minimap_block_size) + (data->settings.minimap_center.y);
	if (get_distance(pos(pixel.x, pixel.y), pos(data->settings.minimap_center.x, data->settings.minimap_center.y)) >
		data->settings.minimap_block_size * data->settings.minimap_render_distance)
		return;
	put_pixel(pixel, data->mlx.game, color);
}

void put_game_pixel(t_vector pixel, int color, t_data *data)
{
	if (data->settings.show_map == false)
		put_pixel(pixel, data->mlx.game, color);
	else if (get_distance(pos(pixel.x, pixel.y),
						  pos(data->settings.minimap_center.x, data->settings.minimap_center.y)) >
			 data->settings.minimap_block_size * data->settings.minimap_render_distance +
				 data->settings.minimap_border_width)
		put_pixel(pixel, data->mlx.game, color);
}

void put_cube(t_pos center, int size, int color, t_data *data)
{
	int x;
	int y;

	y = -size >> 1;
	while (y <= size >> 1)
	{
		x = -size >> 1;
		while (x <= size >> 1)
		{
			put_minimap_pixel(vector(center.x + x, center.y + y), color, data);
			x++;
		}
		y++;
	}
}

void put_ray(t_ray ray, int color, t_data *data)
{
	int i;

	i = 0;
	while (i < ray.distance * data->settings.minimap_block_size && i < data->settings.minimap_size)
	{
		ray.dir = pos(cos(ray.angle) * i, sin(ray.angle) * i);
		if (i && data->settings.minimap_ray_rate != 0 && i % data->settings.minimap_ray_rate == 0)
			put_minimap_pixel(vector(((ray.start.x * data->settings.minimap_block_size) + ray.dir.x),
									 ((ray.start.y * data->settings.minimap_block_size) + ray.dir.y)),
							  color,
							  data);
		i++;
	}
}

void draw_stamina_bar(t_data *data)
{
	if (data->settings.stamina == false)
		return;
	t_settings *settings = &data->settings;
	int			bar_width = settings->screen_width / 6;
	int			bar_height = 20;
	int			bar_x = (settings->screen_width / 2) - (bar_width / 2);
	int			bar_y = settings->screen_height - bar_height - 80;
	float		stamina_ratio = data->player.stamina / data->player.max_stamina;
	int			filled_width = (int)(bar_width * stamina_ratio);

	// background
	for (int y = bar_y; y < bar_y + bar_height && y < settings->screen_height; y++)
	{
		for (int x = bar_x; x < bar_x + bar_width && x < settings->screen_width; x++)
			put_game_pixel(vector(x, y), DARK_BLUE, data);
	}

	// filled portion
	for (int y = bar_y; y < bar_y + bar_height && y < settings->screen_height; y++)
	{
		for (int x = bar_x; x < bar_x + filled_width && x < settings->screen_width; x++)
			put_game_pixel(vector(x, y), GREEN, data);
	}

	// border
	for (int x = bar_x; x < bar_x + bar_width && x < settings->screen_width; x++)
	{
		put_game_pixel(vector(x, bar_y), BLACK, data);					// Top border
		put_game_pixel(vector(x, bar_y + bar_height - 1), BLACK, data); // Bottom border
	}
	for (int y = bar_y; y < bar_y + bar_height && y < settings->screen_height; y++)
	{
		put_game_pixel(vector(bar_x, y), BLACK, data);				   // Left border
		put_game_pixel(vector(bar_x + bar_width - 1, y), BLACK, data); // Right border
	}
}
