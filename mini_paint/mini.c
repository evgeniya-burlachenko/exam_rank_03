#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_list
{
	int		wcanv, hcanv;
	char	bg, type, color;
	float	x, y, radius;
}	t_list;

void free_all(FILE *file, char *str)
{
	fclose(file);
	if (str)
		free(str);
}

int is_rad(int x, int y, t_list list)
{
	float dist;
	dist = sqrtf(((x - list.x) * (x - list.x)) + ((y - list.y) * (y - list.y)));
	if (dist <= list.radius)
	{
		if ((list.radius - dist) < 1)
			return (2);
		return (1);
	}
	return (0);
}

int	drawing(FILE *file, char *draw, t_list list)
{
	int		count;

	while ((count = fscanf(file, "%c %f %f %f %c\n", &list.type, &list.x, &list.y, &list.radius, &list.color)) == 5)
	{
		if (!(list.radius > 0 && (list.type == 'c' || list.type == 'C')))
			return (0);
		int x, y, rad;

		y = -1;
		while (++y < list.hcanv)
		{
			x = -1;
			while (++x < list.wcanv)
			{
				rad = is_rad((float)x, (float)y, list);
				if ((rad == 2 && list.type == 'c') || (rad && list.type == 'C'))
					draw[(y * list.wcanv) + x] = list.color;
			}
		}
	}
	if (count != (-1))
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	FILE		*file;
	t_list		list;
	char		*draw;
	int			i;

	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	if (!(file = fopen(argv[1], "r")))
	{
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	if ((i = fscanf(file, "%d %d %c\n", &list.wcanv, &list.hcanv, &list.bg)) != 3 || i == -1)
	{
		free_all(file, NULL);
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	if (!(list.wcanv > 0 && list.wcanv <= 300 && list.hcanv > 0 && list.hcanv <= 300))
	{
		free_all(file, NULL);
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	if (!(draw = (char *)malloc(sizeof(char) * list.wcanv * list.hcanv))
	 {
		free_all(file, NULL);
		write(1, "Error: Operation file corupted\n", 34);
		return (1);
	}
	i = -1;
	while (++i < (list.wcanv * list.hcanv))
		draw[i] = list.bg;
	if (!(drawing(file, draw, list)))
	{
		free_all(file, draw);
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	i = -1;
	while (++i < list.hcanv)
	{
		write(1, draw + (i * list.wcanv), list.wcanv);
		write(1, "\n", 1);
	}
	free_all(file, draw);
	return (0);
}
