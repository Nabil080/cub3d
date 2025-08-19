#include "cub3d.h"
#include "defines.h"

#define SCREEN_WIDTH	1980
#define SCREEN_HEIGHT	1080
#define MAX_FPS			120
#define FOV				60

#define MAX_SPEED		0.002
#define ACCELERATION	(MAX_SPEED / 10)
#define SPRINT_INCREASE 2.0
#define STAMINA_REGEN	5
#define ROTATION_SPEED	0.005
#define MOUSE_X_SPEED	0.001
#define MOUSE_Y_SPEED	0.1
#define MINIMAP_SIZE	128
#define RENDER_DISTANCE 5
#define BORDER_WIDTH	3
#define RAY_RATE		10

#define SHOW_INPUTS		1
#define SHOW_MAP		1
#define GRID			0
#define LIGHT			1
#define SHOW_RAYS		1
#define HIGHLIGHT_WALLS 1
#define STAMINA			1
#define DEBUG			0

#define INPUT_COLOR		CYAN
#define ESC				65307
#define ENTER			65293
#define ENTER_PAD		65421
#define ONE				65436
#define THREE			65435
#define FOUR			65430
#define SIX				65432
#define SEVEN			65429
#define NINE			65434
#define PLUS			65451
#define MINUS			65453

void update_minimap_settings(t_settings *settings)
{
	if (settings->minimap_render_distance < 1)
		settings->minimap_render_distance = 1;
	settings->minimap_block_size = settings->minimap_size / settings->minimap_render_distance;
	// Ensure block size is at least 1 pixel to avoid rendering issues
	if (settings->minimap_block_size < 1)
		settings->minimap_block_size = 1;
	settings->minimap_center.y = settings->minimap_size + settings->minimap_border_width;
	settings->minimap_center.x = settings->screen_width - (settings->minimap_size + settings->minimap_border_width);
	settings->minimap_full_size = (settings->minimap_size + settings->minimap_border_width) * 2;
	if (settings->minimap_full_size > settings->screen_height / 2)
	{
		settings->minimap_size = (settings->screen_height / 2 - settings->minimap_border_width * 2) / 2;
		settings->minimap_full_size = (settings->minimap_size + settings->minimap_border_width) * 2;
	}
	if (settings->minimap_full_size < 64) // Minimum size to ensure visibility
	{
		settings->minimap_size = (64 - settings->minimap_border_width * 2) / 2;
		settings->minimap_full_size = (settings->minimap_size + settings->minimap_border_width) * 2;
	}
	// Calculate player size
	settings->minimap_player_size = settings->minimap_block_size / 4;
	if (settings->minimap_player_size < 2) // Ensure player is visible
		settings->minimap_player_size = 2;
	printf("Minimap Settings Updated:\n"
		   "\tminimap_size: %d\n"
		   "\tminimap_render_distance: %d\n"
		   "\tminimap_block_size: %d\n"
		   "\tminimap_full_size: %d\n"
		   "\tminimap_center: (%d, %d)\n"
		   "\tminimap_player_size: %d\n",
		   settings->minimap_size,
		   settings->minimap_render_distance,
		   settings->minimap_block_size,
		   settings->minimap_full_size,
		   settings->minimap_center.x,
		   settings->minimap_center.y,
		   settings->minimap_player_size);
}

void update_fov_settings(t_settings *settings)
{
	settings->fov_rad = (double)settings->fov * (PI / 180);
	settings->proj_plane_x = (settings->screen_width / 2.0) / tan(settings->fov_rad / 2.0);
	settings->proj_plane_y = (settings->screen_height / 2.0) / tan(settings->fov_rad / 2.0);
}

void init_settings(t_data *data)
{
	t_settings *settings = &data->settings;
	settings->fov = FOV;
	settings->max_fps = MAX_FPS;
	settings->max_speed = MAX_SPEED;
	settings->acceleration = ACCELERATION;
	settings->rotation_speed = ROTATION_SPEED;
	settings->mouse_speed_x = MOUSE_X_SPEED;
	settings->mouse_speed_y = MOUSE_Y_SPEED;
	settings->sprint_increase = SPRINT_INCREASE;
	settings->stamina_regen = STAMINA_REGEN;
	settings->minimap_size = MINIMAP_SIZE;
	settings->minimap_render_distance = RENDER_DISTANCE;
	settings->minimap_border_width = BORDER_WIDTH;
	settings->minimap_ray_rate = RAY_RATE;
	settings->show_map = SHOW_MAP;
	settings->show_grid = GRID;
	settings->show_rays = SHOW_RAYS;
	settings->light = LIGHT;
	settings->highlight_walls = HIGHLIGHT_WALLS;
	settings->show_inputs = SHOW_INPUTS;
	settings->stamina = STAMINA;
	settings->debug = DEBUG;
	data->player.stamina = 100;
	update_minimap_settings(settings);
	update_fov_settings(settings);
}

static void show_input(t_data *data, char *str)
{
	static const int x = 10;
	static int		 y = 20;

	if (!str)
	{
		y = 20;
		return;
	}
	mlx_string_put(data->mlx.ptr, data->mlx.win, x, y, INPUT_COLOR, str);
	y += 20;
}

void show_inputs(t_data *data)
{
	if (data->settings.show_inputs == false)
		show_input(data, "---------- Inputs | Press Enter to show ----------");
	else
	{
		show_input(data, "---------- Inputs | Press Enter to hide ----------");
		show_input(data, "Move : W/A/S/D");
		show_input(data, "Rotate : Left/Right or Mouse");
		show_input(data, "Sprint : Left Shift");
		show_input(data, "Jump : Space");
		show_input(data, "Crouch : C");
		show_input(data, "Exit: Escape");
		show_input(data, "---- Settings ----");
		show_input(data, "Toggle minimap : M");
		show_input(data, "Toggle light : L");
		show_input(data, "Toggle minimap grid : G");
		show_input(data, "Toggle minimap rays : R");
		show_input(data, "Toggle minimap wall highlights : H");
		show_input(data, "Toggle stamina : I");
		show_input(data, "Toggle DEBUG : B");
		show_input(data, "----");
		show_input(data, "FOV : +/- or Mouse Wheel");
		show_input(data, "Ray rate : 1/3");
		show_input(data, "Map zoom : 4/6");
		show_input(data, "Map size : Up/Down");
		show_input(data, "---------- Inputs | Press Enter to hide ----------");
	}
	show_input(data, NULL);
}

void settings_hooks(int keycode, t_data *data)
{
	t_settings *settings = &data->settings;
	if (keycode == XK_m)
		settings->show_map = !settings->show_map;
	if (keycode == XK_g)
		settings->show_grid = !settings->show_grid;
	if (keycode == XK_l)
		settings->light = !settings->light;
	if (keycode == XK_h)
		settings->highlight_walls = !settings->highlight_walls;
	if (keycode == XK_r)
		settings->show_rays = !settings->show_rays;
	if (keycode == ENTER || keycode == ENTER_PAD)
		settings->show_inputs = !settings->show_inputs;
	if (keycode == XK_i)
		settings->stamina = !settings->stamina;
	if (keycode == XK_b)
		settings->debug = !settings->debug;
	if (keycode == PLUS && settings->fov <= 90)
	{
		settings->fov += 1;
		update_fov_settings(settings);
	}
	if (keycode == MINUS && settings->fov >= 10)
	{
		settings->fov -= 1;
		update_fov_settings(settings);
	}
	if (keycode == XK_Up && settings->minimap_full_size < settings->screen_height)
	{
		settings->minimap_size += 8;
		update_minimap_settings(settings);
	}
	if (keycode == XK_Down && settings->minimap_full_size > 124)
	{
		settings->minimap_size -= 8;
		update_minimap_settings(settings);
	}
	if (keycode == ONE && settings->minimap_ray_rate)
		settings->minimap_ray_rate += 1;
	if (keycode == THREE && settings->minimap_ray_rate > 1)
		settings->minimap_ray_rate -= 1;
	if (keycode == SIX && settings->minimap_render_distance > 1)
	{
		settings->minimap_render_distance -= 1;
		update_minimap_settings(settings);
	}
	if (keycode == FOUR && settings->minimap_render_distance < 16)
	{
		settings->minimap_render_distance += 1;
		update_minimap_settings(settings);
	}
}

static void print_debug(t_data *data, const char *str, bool reset)
{
	static int y = 20;
	const int  x = data->settings.screen_width / 2;

	if (reset)
		y = 20;

	if (str)
	{
		mlx_string_put(data->mlx.ptr, data->mlx.win, x, y, WHITE, (char *)str);
		y += 20;
	}
}

void print_debugs(t_data *data)
{
	static char buffer[200];

	print_debug(data, "--- Screen ---", true);
	snprintf(buffer, sizeof(buffer), "Screen width: %d", data->settings.screen_width);
	print_debug(data, buffer, false);
	snprintf(buffer, sizeof(buffer), "Screen height: %d", data->settings.screen_height);
	print_debug(data, buffer, false);

	print_debug(data, "--- Map ---", false);
	snprintf(buffer, sizeof(buffer), "Map width: %zu", data->map_width);
	print_debug(data, buffer, false);
	snprintf(buffer, sizeof(buffer), "Map height: %zu", data->map_height);
	print_debug(data, buffer, false);

	print_debug(data, "--- Player ---", false);
	snprintf(buffer, sizeof(buffer), "Position: (%f, %f)", data->player.pos.x, data->player.pos.y);
	print_debug(data, buffer, false);
	snprintf(buffer, sizeof(buffer), "Angle: %f", data->player.angle);
	print_debug(data, buffer, false);
	snprintf(buffer, sizeof(buffer), "Velocity: %f", data->player.velocity);
	print_debug(data, buffer, false);
	snprintf(buffer, sizeof(buffer), "Stamina: %f", data->player.stamina);
	print_debug(data, buffer, false);
	snprintf(buffer, sizeof(buffer), "Crouching: %s", data->player.is_crouching ? "true" : "false");
	print_debug(data, buffer, false);
	snprintf(buffer, sizeof(buffer), "Jumping: %s", data->player.is_jumping ? "true" : "false");
	print_debug(data, buffer, false);
	snprintf(buffer, sizeof(buffer), "Sprinting: %s", data->controls.sprint ? "true" : "false");
	print_debug(data, buffer, false);
	snprintf(buffer, sizeof(buffer), "Z tilt: %f", data->player.z_tilt);
	print_debug(data, buffer, false);
	snprintf(buffer, sizeof(buffer), "Z offset: %f", data->player.z_offset);
	print_debug(data, buffer, false);
	snprintf(buffer, sizeof(buffer), "Z velocity: %f", data->player.z_velocity);
	print_debug(data, buffer, false);
}
