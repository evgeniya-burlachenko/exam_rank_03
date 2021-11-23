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

int	drawing(FILE *file, char *draw, t_list list)
{
	int		count, x, y, rec;
	////r - пустой X Y WIDTH HEIGHT CHAR-------проверки--------------------------------
	while ((count = fscanf(file, "%c %f %f %f %f %c\n", &list.type, &list.x, &list.y,
				&list.w, &list.h, &list.color)) == 6)
	{
		if ((list.h <= 0.00000000 || list.w <= 0.00000000) || (list.type != 'r' && list.type != 'R'))
			return (0);
	//---------------------------------------------------------------------------------
		y = -1;
		while (++y < list.hcanv)//height
		{
			x = -1;
			while (++x < list.wcanv)//width
			{
				float	check = 1.00000000;
	//------не входит в диапазон-------------------------------------------------------
				if ((x < list.x) || (list.x + list.w < x) || (y < list.y) || (list.y + list.h < y))
					rec = 0;
	//---------------------------------------------------------------------------------
				else if (((x - list.x) < check) || ((list.x + list.w) - x < check)
					|| ((y - list.y) < check) || ((list.y + list.h) - y < check))
					rec = 2;//border
				else 
					rec = 1;//inside
	//---------------------------------------------------------------------------------
				if ((list.type == 'r' && rec == 2) || (list.type == 'R' && rec))
					(draw)[(y * list.wcanv) + x] = list.color;
				//draw[y * ширина + х]
			}
		}
	}
	if (count != -1)
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	FILE	*file;
	char	*draw;
	t_list	list;
	int		i;
	//-----------------проверки----------------------------------------------------------
	//1
	if (argc != 2)
		return (error(ERR1));
	//2.
	else if (!(file = fopen(argv[1], "r")))
		return (error(ERR2));
	//3. проверка 1ой строки -фон
	else if ((i = fscanf(file, "%d %d %c\n", &list.wcanv, &list.hcanv, &list.bg)) != 3 || i == -1)
			return (free_all(file, NULL) && error(ERR2));
	//4.
	else if ((list.wcanv <= 0 || list.wcanv > 300) || (list.hcanv <= 0 || list.hcanv > 300))
			return (free_all(file, NULL) && error(ERR2));
	//5.
	else if (!(draw = (char *)malloc(sizeof(char) * (list.wcanv * list.hcanv))))
			return (free_all(file, NULL) && error(ERR2));
	//------------------зарисовка bg чарами----------------------------------------------
	i = -1;
	while (++i < list.wcanv * list.hcanv)
		draw[i] = list.bg;
	//-------------------рисуем прямоугольник-------------------------------------------
	if (!(drawing(file, draw, list)))
		return (free_all(file, draw) && error(ERR2));
	//-------------------печать----------------------------------------------------------
	//(draw)[(y * list.wcanv) + x] = list.color;
	i = -1;
	while (++i < list.hcanv)
	{
		write(1, draw + (i * list.wcanv), list.wcanv);
		write(1, "\n", 1);
	}
	//-----------------------------------------------------------------------------------
	free_all(file, draw);
	return (0);
}
