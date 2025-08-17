#include "cub3d.h"

/** Error messages used with defines to get the correct index
 * @example g_error_messages[ERR_MALLOC] = "Allocation error"
 */
static const char *g_error_messages[50] = {
	"undefined",
	"Wrong number of arguments.\nUsage: ./cub3d <map.cub>",
	"Wrong file extension.\nUsage: ./cub3d <map.cub>",
	"No filename.\nUsage: ./cub3d <map.cub>",
	"The file can't be opened.",
	"The file cannot be read from.",
	"The file is empty.",
	"Wrong or missing identifier. (NO,SO,WE,EA,F,C, MAP)",
	"Missing value for an identifier.",
	"Bad identifier value.",
	"Allocation error.",
	"The map is missing.",
	"The map has an invalid character.",
	"The map isn't surrounded by walls.",
	"The map must have one and only one player.",
	"A texture couldn't be loaded",
	NULL,
};

/**
 * @date 20/10/2024
 * @file exit.c
 * @brief Exits the shell with the error code and the corresponding error message
 * @param code error_code as defined in the header,
 * like ERR_MALLOC or ERR_MAP_MISSING
 **/
void exit_error(int code)
{
	if (code == 0)
		return;
	ft_putendl_fd(g_error_messages[code], 2);
	exit(code);
}

/**
 * @date 20/10/2024
 * @file exit.c
 * @brief Frees t_data then exits the shell with the error code
 * and the corresponding error message.
 * @param code error_code as defined in the header,
 * like ERR_MALLOC or ERR_MAP_MISSING
 **/
void exit_free(int code, t_data *data)
{
	free_data(data);
	exit_error(code);
}
/**
 * @date 22/10/2024
 * @file exit.c
 * @brief destroy everything mlx related (image, window...)
 **/
static void free_mlx(t_mlx_data mlx, t_data *data)
{
	size_t i;

	i = 0;
	while (i < 6)
	{
		if (data->img[i].img)
			mlx_destroy_image(data->mlx.ptr, data->img[i].img);
		i++;
	}
	if (mlx.win)
		mlx_destroy_window(mlx.ptr, mlx.win);
	mlx_destroy_display(mlx.ptr);
	free(mlx.ptr);
}

/**
 * @date 20/10/2024
 * @file exit.c
 * @brief Frees the pointer which aren't pointing to NULL
 **/
void free_data(t_data *data)
{
	size_t i;

	i = 0;
	while (i < 6)
	{
		if (data->s_textures[i])
			free(data->s_textures[i]);
		i++;
	}
	if (data->mlx.ptr)
		free_mlx(data->mlx, data);
	if (data->map != NULL)
		free_2d((void *)data->map, 0);
	if (data->tmp != NULL)
		free(data->tmp);
	if (data->fd > 0)
		close(data->fd);
}

/**
 * @date 20/10/2024
 * @file exit.c
 * @brief Exits the program successfully
 **/
int exit_game(t_data *data)
{
	printf("Exiting the game\n");
	if (data)
		free_data(data);
	exit(0);
	return (0);
}
