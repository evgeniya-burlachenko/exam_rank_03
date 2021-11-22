#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ERR1 "Error: argument\n"
#define ERR2 "Error: Operation file corrupted\n"

typedef struct s_list
{
	int		wcanv, hcanv;
	char	bg, type, color;
	float	x, y, w, h;
	
}	t_list;

int	error(char *err)
{
	int	i = 0;

	while (err[i])
		i++;
	write(1, err, i);
	return (1);
}

int	free_all(FILE *file, char *draw)
{
	fclose(file);
	if (draw)
		free(draw);
	return (1);
}

int	is_rec(float y, float x, t_list *tmp)
{
	float	check = 1;

	if ((x < tmp->x) || (tmp->x + tmp->w < x) || (y < tmp->y) || (tmp->y + tmp->h < y))
		return (0);
	if (((x - tmp->x) < check) || ((tmp->x + tmp->w) - x < check)
		|| ((y - tmp->y) < check) || ((tmp->y + tmp->h) - y < check))
		return (2);
	return (1);
}

int	drawing(FILE *file, char *draw, t_list list)
{
	int		count;

	while ((count = fscanf(file, "%c %f %f %f %f %c\n", &list.type, &list.x, &list.y,
				&list.w, &list.h, &list.color)) == 6)
	{
		if (!((list.h > 0 && list.w > 0)
			&& (list.type == 'r' || list.type == 'R')))
			return (0);
		int	x, y, rec;

		y = -1;
		while (++y < list.hcanv)
		{
			x = -1;
			while (++x < list.wcanv)
			{
				rec = is_rec(y, x, &list);
				if ((list.type == 'r' && rec == 2) || (list.type == 'R' && rec))
					(draw)[(y * list.wcanv) + x] = list.color;
			}
		}
	}
	if (count != (-1))
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	FILE	*file;
	char	*draw;
	t_list	list;
	int		i;

	if (argc != 2)
		return (error(ERR1));
	else if (!(file = fopen(argv[1], "r")))
		return (error(ERR2));
	else if ((i = fscanf(file, "%d %d %c\n", &list.wcanv, &list.hcanv, &list.bg)) != 3)
			return (free_all(file, NULL) && error(ERR2));
	else if (i == (-1))
			return (free_all(file, NULL) && error(ERR2));
	else if (!(list.wcanv >= 0 && list.wcanv <= 300) && (list.hcanv >= 0 && list.hcanv <= 300))
			return (free_all(file, NULL) && error(ERR2));
	else if (!(draw = (char *)malloc(sizeof(char) * (list.wcanv * list.hcanv))))
			return (free_all(file, NULL) && error(ERR2));
	i = -1;
	while (++i < list.wcanv * list.hcanv)
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
