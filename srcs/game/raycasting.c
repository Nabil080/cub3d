#include "cub3d.h"

static void get_vertical_intersection(t_ray *ray, t_data *data)
{
	double px = 1e-6;
	ray->step.x = 1;
	ray->step.y = tan(ray->angle);
	if (ray->angle > PI / 2 && ray->angle < (3 * PI) / 2)
	{
		ray->step.x *= -1;
		ray->step.y *= -1;
		px = -px;
		ray->v_inter.x = floor(ray->start.x) + px;
	}
	else
		ray->v_inter.x = ceil(ray->start.x) + px;
	ray->v_inter.y = ray->start.y + (ray->v_inter.x - ray->start.x) * tan(ray->angle);
	while (!is_wall(data, ray->v_inter.x, ray->v_inter.y))
	{

		if (ray->step.x < 0 && is_wall(data, ray->v_inter.x - px, ray->v_inter.y))
			break;
		ray->v_inter.x += ray->step.x;
		ray->v_inter.y += ray->step.y;
	}
	ray->v_inter.x -= px;
}

static void get_horizontal_intersection(t_ray *ray, t_data *data)
{
	double px = 1e-6;
	ray->step.y = 1;
	ray->step.x = 1.0 / tan(ray->angle);
	if (ray->angle < PI && ray->angle > 0)
		ray->h_inter.y = ceil(ray->start.y) + px;
	else
	{
		ray->step.y *= -1;
		ray->step.x *= -1;
		px = -px;
		ray->h_inter.y = floor(ray->start.y) + px;
	}
	ray->h_inter.x = ray->start.x + (ray->h_inter.y - ray->start.y) / tan(ray->angle);
	while (!is_wall(data, ray->h_inter.x, ray->h_inter.y))
	{
		if (ray->step.y < 0 && is_wall(data, ray->h_inter.x, ray->h_inter.y - px))
			break;
		ray->h_inter.x += ray->step.x;
		ray->h_inter.y += ray->step.y;
	}
	ray->h_inter.y -= px;
}

t_ray cast_ray(t_data *data, t_pos start, double angle)
{
	t_ray ray;
	ray.start = start;
	ray.angle = angle;
	ray.dir = pos(cos(angle), sin(angle));
	get_horizontal_intersection(&ray, data);
	get_vertical_intersection(&ray, data);
	if (get_distance(ray.start, ray.h_inter) > get_distance(ray.start, ray.v_inter))
	{
		ray.end = ray.v_inter;
		if (PI / 2 < ray.angle && ray.angle < (3 * PI) / 2)
			ray.hit = 'W';
		else
			ray.hit = 'E';
	}
	else
	{
		ray.end = ray.h_inter;
		if (PI > ray.angle && ray.angle > 0)
			ray.hit = 'S';
		else
			ray.hit = 'N';
	}
	ray.distance = get_distance(ray.start, ray.end);
	return ray;
}
