#include "cub3d.h"

static void put_minimap_border(t_data *data)
{
	t_pos  i;
	double distance;

	i.y = 0;
	while (i.y < (data->settings.minimap_center) * 2)
	{
		i.x = 0;
		while (i.x < (data->settings.minimap_center) * 2)
		{
			distance = get_distance(i, pos(data->settings.minimap_center, data->settings.minimap_center));
			if (distance > data->settings.minimap_size &&
				distance < data->settings.minimap_border_width + data->settings.minimap_size)
				put_pixel(vector((int)i.x, (int)i.y), data->mlx.game, WHITE);
			i.x++;
		}
		i.y++;
	}
}

static void put_minimap_block(t_data *data, t_vector cell)
{
	t_vector pixel;
	t_vector i;

	i.y = data->settings.show_grid;
	while (i.y < data->settings.minimap_block_size)
	{
		i.x = data->settings.show_grid;
		while (i.x < data->settings.minimap_block_size)
		{
			pixel.x = (cell.x * data->settings.minimap_block_size) + i.x;
			pixel.y = (cell.y * data->settings.minimap_block_size) + i.y;
			if (data->map[cell.y][cell.x] == WALL)
				put_minimap_pixel(pixel, WALL_COLOR, data);
			else if (data->map[cell.y][cell.x] == FLOOR)
				put_minimap_pixel(pixel, FLOOR_COLOR, data);
			i.x++;
		}
		i.y++;
	}
}

void draw_minimap(t_data *data)
{
	t_vector cell;

	cell.y = 0;
	if (data->player.pos.y > data->settings.minimap_render_distance)
		cell.y = (int)data->player.pos.y - data->settings.minimap_render_distance;
	while (data->settings.light && cell.y < data->player.pos.y + data->settings.minimap_render_distance &&
		   data->map[cell.y])
	{
		cell.x = 0;
		if (data->player.pos.x > data->settings.minimap_render_distance)
			cell.x = (int)data->player.pos.x - data->settings.minimap_render_distance;
		while (cell.x < data->player.pos.x + data->settings.minimap_render_distance && data->map[cell.y][cell.x])
		{
			put_minimap_block(data, cell);
			cell.x++;
		}
		cell.y++;
	}
	put_minimap_border(data);
	if (data->settings.light)
		put_cube(pos(data->player.pos.x * data->settings.minimap_block_size,
					 data->player.pos.y * data->settings.minimap_block_size),
				 data->settings.minimap_player_size,
				 PLAYER_COLOR,
				 data);
	else
		put_cube(pos(data->player.pos.x * data->settings.minimap_block_size,
					 data->player.pos.y * data->settings.minimap_block_size),
				 data->settings.minimap_player_size,
				 GRAY,
				 data);
}
