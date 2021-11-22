#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define ERR1 "Error: argument\n"
#define ERR2 "Error: Operation file corrupted\n"

typedef struct s_list
{
	int		wcanv, hcanv;
	char	bg, type, color;
	float	x, y, radius;
}	t_list;

int	error(char *err)
{
	int	i = 0;

	while (err[i] != '\0')
		i++;
	write(1, err, i);
	return (1);
}

int	free_all(FILE *file, char *str)
{
	fclose(file);
	if (str)
		free(str);
	return (1);
}

int	drawing(FILE *file, char *draw, t_list list)
{
	int		count;

	while ((count = fscanf(file, "%c %f %f %f %c\n", &list.type, &list.x, &list.y, &list.radius, &list.color)) == 5)
	{
		if (list.radius <= 0 && (list.type != 'c' || list.type != 'C'))
			return (0);
		int x, y, rad;

		y = -1;
		while (++y < list.hcanv)
		{
			x = -1;
			while (++x < list.wcanv)
			{
				float dist;
				dist = sqrtf(((x - list.x) * (x - list.x)) + ((y - list.y) * (y - list.y)));
				if (dist <= list.radius)
				{
					if ((list.radius - dist) < 1)
						rad = 2;
					else
						rad = 1;
				}
				else 
				rad = 0;
				if ((rad == 2 && list.type== 'c') || (rad && list.type== 'C'))
					draw[(y * list.wcanv) + x] =  list.color;
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
		return (error(ERR1));
	else if (!(file = fopen(argv[1], "r")))
		return (error(ERR2));
	else if ((i = fscanf(file, "%d %d %c\n", &list.wcanv, &list.hcanv, &list.bg)) != 3 || i == -1)
		return (free_all(file, NULL) && error(ERR2));
	if (list.wcanv <= 0 || list.wcanv > 300 || list.hcanv <= 0 || list.hcanv > 300)
		return (free_all(file, NULL) && error(ERR2));
	if (!(draw= (char *)malloc(sizeof(char) * (list.wcanv * list.hcanv))))
		return (free_all(file, NULL) && error(ERR2));
	i = -1;
	while (++i < (list.wcanv * list.hcanv))
		draw[i] = list.bg;
	if (!(drawing(file, draw, list)))
		return (free_all(file, draw) && error(ERR2));
	i = -1;
	while (++i < list.hcanv)
	{
		write(1, draw + (i * list.wcanv), list.wcanv);
		write(1, "\n", 1);
	}
	free_all(file, draw);
	return (0);
}
