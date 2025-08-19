#include "cub3d.h"

static int get_wall_pixel(t_data *data, t_ray ray, int j, int wall_height)
{
	t_img	 texture;
	t_vector pixel;
	double	 tex_pos;
	int		 color;

	if (ray.hit == 'W')
		texture = data->img[T_WEST];
	else if (ray.hit == 'E')
		texture = data->img[T_EAST];
	else if (ray.hit == 'S')
		texture = data->img[T_SOUTH];
	else
		texture = data->img[T_NORTH];

	if (ray.hit == 'N' || ray.hit == 'S')
		tex_pos = ray.end.x - floor(ray.end.x);
	else
		tex_pos = ray.end.y - floor(ray.end.y);

	pixel.x = (int)(tex_pos * texture.width);
	pixel.y = (int)((j * texture.height) / wall_height);

	color = *(unsigned int *)(texture.addr + pixel.y * texture.line_length + pixel.x * (texture.bits_per_pixel / 8));

	// darken by 50% for South and East walls to simulate shadow
	if (ray.hit == 'S' || ray.hit == 'E')
	{
		unsigned char r = (color >> 16) & 0xFF;
		unsigned char g = (color >> 8) & 0xFF;
		unsigned char b = color & 0xFF;
		unsigned char a = (color >> 24) & 0xFF;

		// reduce brightness by 50%
		r = (unsigned char)(r * 0.5);
		g = (unsigned char)(g * 0.5);
		b = (unsigned char)(b * 0.5);

		color = (a << 24) | (r << 16) | (g << 8) | b;
	}

	return (color);
}

static int get_direction_color(char direction)
{
	if (direction == 'W')
		return (YELLOW);
	else if (direction == 'E')
		return (RED);
	else if (direction == 'S')
		return (CYAN);
	else if (direction == 'N')
		return (PURPLE);
	return (BLACK);
}

static void render_wall(t_data *data, t_ray ray, int i)
{
	int wall_height;
	int ceiling_size;
	int y;
	int j;
	int z;

	// Calculate wall height based on ray distance
	wall_height = (data->settings.proj_plane_y / ray.distance);

	// Calculate ceiling size, adjusted by player's z_offset (for jumping/crouching)
	ceiling_size = (data->settings.screen_height >> 1) - (wall_height >> 1);
	ceiling_size -= (data->player.z_tilt * 10);
	ceiling_size += (int)(data->player.z_offset * wall_height);

	// Render ceiling
	y = 0;
	while (y < data->settings.screen_height && y < ceiling_size)
		put_game_pixel(vector(i, y++), data->conv_ceiling, data);

	// Handle wall rendering
	z = 0;
	if (ceiling_size < 0)
		z = -ceiling_size; // Adjust for clipping when ceiling is off-screen

	j = -1;
	while (y + ++j < data->settings.screen_height && y + j < ceiling_size + wall_height)
		put_game_pixel(vector(i, y + j), get_wall_pixel(data, ray, j + z, wall_height), data);

	// Update y to skip to the floor
	y += j;

	// Render floor
	while (y < data->settings.screen_height)
		put_game_pixel(vector(i, y++), data->conv_floor, data);
}

void draw_game(t_data *data)
{
	double angle;
	double step;
	t_ray  ray;
	int	   i;

	angle = data->player.angle - data->settings.fov_rad / 2;
	step = data->settings.fov_rad / data->settings.screen_width;
	i = 0;
	while (i <= data->settings.screen_width)
	{
		ray = cast_ray(data, data->player.pos, nor_angle(angle));
		if (data->settings.show_map && data->settings.highlight_walls)
			put_cube(pos(ray.end.x * data->settings.minimap_block_size, ray.end.y * data->settings.minimap_block_size),
					 1,
					 get_direction_color(ray.hit),
					 data);
		if (data->settings.show_map && data->settings.show_rays)
			put_ray(ray, WHITE, data);
		ray.distance *= cos(data->player.angle - ray.angle);
		if (data->settings.light)
			render_wall(data, ray, i);
		angle += step;
		i++;
	}
}
