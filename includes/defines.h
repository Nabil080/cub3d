#ifndef DEFINES_H
#define DEFINES_H

/*game*/
#include <stdbool.h>
#include <stddef.h>
#define TITLE			  "cub3d"

/*cells*/
#define FLOOR			  '0'
#define WALL			  '1'
#define SPACE			  ' '
/*consts*/
#define PI				  3.14159265358979323846
#define PX				  0.0001
#define FPS_INTERVAL	  1000
/*colors*/
#define PLAYER_COLOR	  RED
#define WALL_COLOR		  BLACK
#define FLOOR_COLOR		  GRAY

#define BLUE			  0xFF428f77
#define DARK_BLUE		  0x143143
#define PURPLE			  0xa134eb
#define GREEN			  0x02D05D
#define RED				  0xFF0000
#define CYAN			  0x00FFFF
#define YELLOW			  0xFFFF00
#define ORANGE			  0xFFFFA500
#define WHITE			  0xFFFFFF
#define BLACK			  0x000000
#define GRAY			  0x808080
#define BROWN			  0xFF8B4513
/*error codes*/
#define ERR_UNDEFINED	  0
#define ERR_ARG_COUNT	  1
#define ERR_ARG_EXT		  2
#define ERR_ARG_NAME	  3
#define ERR_FILE_OPEN	  4
#define ERR_FILE_READ	  5
#define ERR_FILE_EMPTY	  6
#define ERR_ID_MISSING	  7
#define ERR_VALUE_MISSING 8
#define ERR_VALUE_ERROR	  8
#define ERR_MALLOC		  10
#define ERR_MAP_MISSING	  11
#define ERR_MAP_CHAR	  12
#define ERR_MAP_WALL	  13
#define ERR_MAP_PLAYER	  14
#define ERR_TEXTURE		  15
/*images*/
#define T_FLOOR			  0
#define T_CEILING		  1
#define T_NORTH			  2
#define T_SOUTH			  3
#define T_WEST			  4
#define T_EAST			  5
/*structs*/
typedef struct s_pos
{
		double x;
		double y;
} t_pos;

typedef struct s_vector
{
		int x;
		int y;
} t_vector;

typedef struct s_player
{
		t_pos  pos;
		double angle;
		double z_tilt;
		double velocity;
		double z_offset;

		bool   is_jumping;
		bool   is_crouching;
		double z_velocity;
} t_player;

typedef struct s_textures
{
		void *im_wall;
		void *im_floor;
		void *im_player;
		void *im_sniper;
		void *im_scope;
} t_textures;

typedef struct s_controls
{

		bool left;
		bool right;
		bool up;
		bool down;
		bool sprint;
		bool w;
		bool a;
		bool s;
		bool d;
		bool crouch;
		bool jump;
		int	 l_r;
		int	 u_d;
} t_controls;

typedef struct s_img
{
		void *img;
		char *addr;
		int	  bits_per_pixel;
		int	  line_length;
		int	  endian;
		t_pos center;
		int	  width;
		int	  height;
} t_img;

typedef struct s_mlx_data
{
		void	 *ptr;
		void	 *win;
		t_img	  game;
		long long last_frame;
		long long delta_time;
} t_mlx_data;

typedef struct s_settings
{
		// const
		int		 screen_width;
		int		 screen_height;

		// Changeable settings
		int		 max_fps; // 0 - 999
		int		 fov;	  // 20-80

		// Might change so values make sense
		double	 max_speed;				  // 0.002
		double	 acceleration;
		double	 rotation_speed;		  // 0.005
		double	 mouse_speed_x;			  // 0.001
		double	 mouse_speed_y;			  // 0.1
		float	 sprint_increase;		  // 1.5

		int		 minimap_size;			  // 128 px
		int		 minimap_render_distance; // 4 blocks
		int		 minimap_border_width;	  // 1 px
		int		 minimap_ray_rate;		  // every 10px

		bool	 show_map;
		bool	 show_grid;
		bool	 light;
		bool	 show_rays;
		bool	 highlight_walls;
		bool	 show_inputs;

		// Adaptative settings
		double	 fov_rad;			  // Changes with fov
		double	 proj_plane_x;		  // Changes with fov
		double	 proj_plane_y;		  // Changes with fov

		t_vector minimap_center;	  // changes with minimap size/border_width
		int		 minimap_block_size;  // changes with minimap size/render_distance
		int		 minimap_full_size;	  // changes with minimap size/border_width
		int		 minimap_player_size; // changes with minimap_block_size;
} t_settings;

typedef struct s_data
{

		int		   fd;
		char	 **map;
		size_t	   map_width;
		size_t	   map_height;
		char	  *s_textures[6];
		int		   conv_floor;
		int		   conv_ceiling;
		t_textures textures;
		t_settings settings;
		t_mlx_data mlx;
		t_player   player;
		t_controls controls;
		t_img	   img[6];
		char	  *tmp;
} t_data;

typedef struct s_ray
{
		t_pos  start;
		t_pos  dir;
		t_pos  end;
		t_pos  h_inter;
		t_pos  v_inter;
		t_pos  step;
		double distance;
		double angle;
		char   hit;
} t_ray;

typedef enum e_timetype
{
	S,
	MS,
	US
} t_timetype;

#endif
