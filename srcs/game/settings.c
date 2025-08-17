#include "cub3d.h"
#include "defines.h"

/* changeable */
#define SCREEN_WIDTH	   1980
#define SCREEN_HEIGHT	   1080
#define MAX_FPS			   120
#define FOV				   60

#define MAX_SPEED		   0.002
#define ACCELERATION	   (MAX_SPEED / 10)
#define SPRINT_INCREASE	   1.5
#define ROTATION_SPEED	   0.005
#define MOUSE_X_SPEED	   0.001
#define MOUSE_Y_SPEED	   0.1
#define MINIMAP_SIZE	   128
#define RENDER_DISTANCE	   4
#define BORDER_WIDTH	   1
#define RAY_RATE		   10

/* settings */
#define DEBUG			   0
#define SHOW_MAP		   1
#define GRID			   1
#define LIGHT			   1
#define SHOW_RAYS		   1
#define HIGHLIGHT_WALLS	   1

/* adaptative */
#define MINIMAP_BLOCK_SIZE abs(MINIMAP_SIZE / RENDER_DISTANCE)
#define MINIMAP_CENTER	   (MINIMAP_SIZE + BORDER_WIDTH)
#define MINIMAP_FULL_SIZE  ((MINIMAP_SIZE + BORDER_WIDTH) * 2)
#define PLAYER_SIZE		   (MINIMAP_BLOCK_SIZE / 4)
#define FOV_RAD			   ((double)FOV * (PI / 180))
#define PROJ_PLANE_X	   ((SCREEN_WIDTH / 2.0) / tan(FOV_RAD / 2.0))
#define PROJ_PLANE_Y	   ((SCREEN_HEIGHT / 2.0) / tan(FOV_RAD / 2.0))

static void update_minimap_settings(t_settings *settings)
{
	settings->minimap_block_size = MINIMAP_BLOCK_SIZE;
	settings->minimap_center = MINIMAP_CENTER;
	settings->minimap_full_size = MINIMAP_FULL_SIZE;
	settings->minimap_player_size = PLAYER_SIZE;
	printf("\
        settings->minimap_block_size = %d\n\
        settings->minimap_center = %d\n\
        settings->minimap_full_size = %d\n\
        settings->minimap_player_size = %d\n\
		   ",
		   settings->minimap_block_size,
		   settings->minimap_center,
		   settings->minimap_full_size,
		   settings->minimap_player_size);
}

static void update_fov_settings(t_settings *settings)
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
	update_minimap_settings(settings);
	update_fov_settings(settings);
}
