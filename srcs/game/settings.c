#include "cub3d.h"
#include "defines.h"

#define SCREEN_WIDTH	1980
#define SCREEN_HEIGHT	1080
#define MAX_FPS			120
#define FOV				60

#define MAX_SPEED		0.002
#define ACCELERATION	(MAX_SPEED / 10)
#define SPRINT_INCREASE 1.5
#define ROTATION_SPEED	0.005
#define MOUSE_X_SPEED	0.001
#define MOUSE_Y_SPEED	0.1
#define MINIMAP_SIZE	128
#define RENDER_DISTANCE 4
#define BORDER_WIDTH	2
#define RAY_RATE		10

#define SHOW_INPUTS		1
#define SHOW_MAP		1
#define GRID			1
#define LIGHT			1
#define SHOW_RAYS		1
#define HIGHLIGHT_WALLS 1

#define INPUT_COLOR		CYAN
#define ENTER			65293
#define ENTER_PAD		65421
#define PLUS			65451
#define MINUS			65453

void update_minimap_settings(t_settings *settings)
{
	settings->minimap_block_size = (settings->minimap_size / settings->minimap_render_distance);
	settings->minimap_center.y = settings->minimap_size + settings->minimap_border_width;
	settings->minimap_center.x = settings->screen_width - (settings->minimap_size + settings->minimap_border_width);
	settings->minimap_full_size = (settings->minimap_size + settings->minimap_border_width) * 2;
	settings->minimap_player_size = settings->minimap_block_size / 4;
	printf("\
        settings->minimap_block_size = %d\n\
        settings->minimap_full_size = %d\n\
        settings->minimap_center = (%d,%d)\n\
        settings->minimap_player_size = %d\n\
		   ",
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
		show_input(data, "---- Settings ----");
		show_input(data, "Toggle minimap : M");
		show_input(data, "Toggle light : L");
		show_input(data, "Toggle minimap grid : G");
		show_input(data, "Toggle minimap rays : R");
		show_input(data, "Toggle minimap wall highlights : H");
		show_input(data, "----");
		show_input(data, "Increase/Decrease FOV : +/- or Mouse Wheel");
		show_input(data, "Increase/Decrease Map size : Up/Down");
	}
	show_input(data, NULL);
}

void settings_hooks(int keycode, t_data *data)
{
	if (keycode == XK_m)
		data->settings.show_map = !data->settings.show_map;
	if (keycode == XK_g)
		data->settings.show_grid = !data->settings.show_grid;
	if (keycode == XK_l)
		data->settings.light = !data->settings.light;
	if (keycode == XK_h)
		data->settings.highlight_walls = !data->settings.highlight_walls;
	if (keycode == XK_r)
		data->settings.show_rays = !data->settings.show_rays;
	if (keycode == ENTER || keycode == ENTER_PAD)
		data->settings.show_inputs = !data->settings.show_inputs;
	if (keycode == PLUS && data->settings.fov <= 90)
	{
		data->settings.fov += 1;
		update_fov_settings(&data->settings);
	}
	if (keycode == MINUS && data->settings.fov >= 10)
	{
		data->settings.fov -= 1;
		update_fov_settings(&data->settings);
	}
	if (keycode == XK_Up && data->settings.minimap_full_size < data->settings.screen_height)
	{
		data->settings.minimap_size += 10;
		update_minimap_settings(&data->settings);
	}
	if (keycode == XK_Down && data->settings.minimap_full_size > 124)
	{
		data->settings.minimap_size -= 10;
		update_minimap_settings(&data->settings);
	}
}
