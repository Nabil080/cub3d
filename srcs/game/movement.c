#include "cub3d.h"

bool is_wall(t_data *data, int x, int y)
{
	if (x < 0 || y < 0)
		return (true);
	if (y >= (int)data->map_height || x >= (int)data->map_width)
		return (true);
	if (data->map[y][x] && data->map[y][x] == WALL)
		return (true);
	return (false);
}

static void safe_move(t_data *data, double x_multiplicator, double y_multiplicator)
{
	t_pos  new_pos;
	double increment;

	if (x_multiplicator == 0 && y_multiplicator == 0)
		return;
	increment = data->player.velocity * data->mlx.delta_time;
	if (data->controls.sprint)
		increment *= data->settings.sprint_increase;
	new_pos.x = data->player.pos.x + (increment * x_multiplicator);
	new_pos.y = data->player.pos.y + (increment * y_multiplicator);
	if (!is_wall(data, (int)new_pos.x, (int)new_pos.y))
		data->player.pos = new_pos;
}

void move_player(t_data *data)
{
	if (data->controls.l_r == 0 && data->controls.u_d == 0 && data->player.velocity > 0)
		data->player.velocity -= data->settings.acceleration;
	else if (data->player.velocity < data->settings.max_speed)
		data->player.velocity += data->settings.acceleration;
	safe_move(data, -(data->controls.l_r) * sin(data->player.angle), 0);
	safe_move(data, 0, data->controls.l_r * cos(data->player.angle));
	safe_move(data, -(data->controls.u_d) * cos(data->player.angle), 0);
	safe_move(data, 0, -(data->controls.u_d) * sin(data->player.angle));
}

void rotate_player(t_data *data)
{
	if (data->controls.left == true)
		data->player.angle -= data->settings.rotation_speed * data->mlx.delta_time;
	if (data->controls.right == true)
		data->player.angle += data->settings.rotation_speed * data->mlx.delta_time;
	data->player.angle = nor_angle(data->player.angle);
}

void jump_player(t_data *data)
{
	const float gravity = -8.0;								// Gravity acceleration
	const float jump_impulse = 2.5;							// Initial upward velocity for jump
	const float crouch_target = -0.25;						// Target z_offset for crouching
	const float crouch_speed = 5.0;							// Speed of crouching transition
	const float ground_height = 0.0;						// Normal standing height
	float		delta_time = data->mlx.delta_time / 1000.0; // Convert ms to seconds

	// Initiate jump
	if (data->controls.jump && !data->player.is_jumping && !data->player.is_crouching)
	{
		data->player.is_jumping = true;
		data->player.z_velocity = jump_impulse;
	}

	// Handle crouching
	if (data->controls.crouch && !data->player.is_jumping)
		data->player.is_crouching = true;
	else if (!data->controls.crouch)
		data->player.is_crouching = false;

	// Update jumping state
	if (data->player.is_jumping)
	{
		data->player.z_velocity += gravity * delta_time;
		data->player.z_offset += data->player.z_velocity * delta_time;

		// Check for landing
		if (data->player.z_offset <= ground_height && data->player.z_velocity < 0)
		{
			data->player.z_offset = ground_height;
			data->player.z_velocity = 0.0;
			data->player.is_jumping = false;
		}
	}
	else if (data->player.is_crouching)
	{
		// Smoothly transition to crouch position
		float target_z_offset = crouch_target;
		data->player.z_offset += (target_z_offset - data->player.z_offset) * crouch_speed * delta_time;
		data->player.z_velocity = 0.0f;
	}
	else
	{
		// Smoothly return to standing position
		data->player.z_offset += (ground_height - data->player.z_offset) * crouch_speed * delta_time;
		data->player.z_velocity = 0.0f;
	}

	// Clamp z_offset to prevent extreme values
	if (data->player.z_offset < crouch_target)
		data->player.z_offset = crouch_target;
	if (data->player.z_offset > 1.0f) // Max jump height
		data->player.z_offset = 1.0f;
}
