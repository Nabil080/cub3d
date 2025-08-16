#include "cub3d.h"

static int get_wall_pixel(t_data *data, t_ray ray, int j, int wall_height)
{
	t_img	 texture;
	t_vector pixel;
	double	 tex_pos;
	int		 color;

	// pick texture
	if (ray.hit == 'W')
		texture = data->img[T_WEST];
	else if (ray.hit == 'E')
		texture = data->img[T_EAST];
	else if (ray.hit == 'S')
		texture = data->img[T_SOUTH];
	else
		texture = data->img[T_NORTH];

	// X coordinate: depends on which side was hit
	if (ray.hit == 'N' || ray.hit == 'S')
		tex_pos = ray.end.x - floor(ray.end.x);
	else
		tex_pos = ray.end.y - floor(ray.end.y);

	pixel.x = (int)(tex_pos * texture.width);
	pixel.y = (int)((j * texture.height) / wall_height);

	color = *(unsigned int *)(texture.addr + pixel.y * texture.line_length + pixel.x * (texture.bits_per_pixel / 8));
	return (color);
}

static int get_direction_color(char direction)
{
	if (direction == 'W')
		return (YELLOW);
	else if (direction == 'E')
		return (DARK_BLUE);
	else if (direction == 'S')
		return (WHITE);
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

	wall_height = (PROJ_PLANE_Y / ray.distance);
	ceiling_size = (data->mlx.window_height >> 1) - (wall_height >> 1);
	ceiling_size -= (data->player.z_tilt * 10);
	y = 0;
	while (y <= data->mlx.window_height && y <= ceiling_size)
		put_game_pixel(vector(i, y++), data->conv_ceiling, data);
	z = 0;
	if (ceiling_size < 0)
		z = -ceiling_size;
	j = -1;
	while (y + ++j <= data->mlx.window_height && y + j <= ceiling_size + wall_height)
	{
		put_game_pixel(vector(i, y + j), get_wall_pixel(data, ray, j + z, wall_height), data);
	}
	y += j;
	while (y <= data->mlx.window_height)
		put_game_pixel(vector(i, y++), data->conv_floor, data);
}

void draw_game(t_data *data)
{
	double angle;
	double step;
	t_ray  ray;
	int	   i;

	angle = data->player.angle - FOV_RAD / 2;
	step = FOV_RAD / data->mlx.window_width;
	i = 0;
	while (i <= data->mlx.window_width)
	{
		ray = cast_ray(data, data->player.pos, nor_angle(angle));
		if (SHOW_MAP && HIGHLIGHT_WALLS)
			put_cube(pos(ray.end.x * MINIMAP_BLOCK_SIZE, ray.end.y * MINIMAP_BLOCK_SIZE),
					 1,
					 get_direction_color(ray.hit),
					 data);
		if (SHOW_MAP && SHOW_RAYS)
			put_ray(ray, WHITE, data);
		ray.distance *= cos(data->player.angle - ray.angle);
		if (LIGHT)
			render_wall(data, ray, i);
		angle += step;
		i++;
	}
}
