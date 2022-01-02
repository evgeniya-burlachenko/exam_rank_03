#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_list
{
	int		wcanv, hcanv;
	char	bg, type, ch;
	float	x, y, w, h;
	
}	t_list;

void	free_all(FILE *file, char *draw)
{
	fclose(file);
	if (draw)
		free(draw);
}

int	is_rec(int x, int y, t_list list)
{//------не входит в диапазон-------------------------------------------------------
	if (x < list.x || list.x + list.w < x || y < list.y \
	|| list.y + list.h < y)
		return 0;
	//------диапазон от 0 до 1------------------------------------------------------------------------
	else if (x - list.x < 1 || list.x + list.w - x < 1 \
	|| y - list.y < 1 || list.y + list.h - y < 1)
		return 2; //border
return 1; //inside
}

int drawing(FILE *file, char *draw, t_list list)
{
	int i, x, y, flag;
	////r - пустой X Y WIDTH HEIGHT CHAR-------
	//-----------fscanf--------------------------------
	while ((i = fscanf(file, "%c %f %f %f %f %c\n", \
	&list.type, &list.x, &list.y, &list.w, &list.h, &list.ch)) \
	== 6)
	{
		//--------диапазон && тип ----------------
		if (!((list.h > 0 && list.w > 0) && (list.type == 'r' || list.type == 'R')))
			return (0);
		//---------------------------------------------------------------------------------
		y = -1;
		while (++y < list.hcanv) //height
		{
			x = -1;
			while (++x < list.wcanv) //width
			{
				flag = is_rec(x, y, list);
				//----------если r и 2 - пустой || если R и 1 или 2 - заполненный----------------------------------------------------------------
				if ((list.type == 'r' && flag == 2) || \
				(list.type == 'R' && flag))
					draw[y * list.wcanv + x] = list.ch;
				//draw[y * ширина + х]
			}
		}
	}
	if (i != -1)
		return (0);
	return (1);
}

int main(int argc, char **argv)
{
	FILE *file;
	char *draw;
	t_list list;
	int i;
	//-----------------проверки----------------------------------------------------------
	//1-------2 аргумента-----------------------
	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	//2-------fopen-----------------------------
	if (!(file = fopen(argv[1], "r")))
	{
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	//3-------fscanf----------------------------
	if ((i = fscanf(file, "%d %d %c\n", \
	&list.wcanv, &list.hcanv, &list.bg)) != 3 || i == -1)
	{
		free_all(file, NULL);
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	//4------диапазон---------------------------
	if (!(list.wcanv > 0 && list.wcanv <= 300 \
	&& list.hcanv > 0 && list.hcanv <= 300))
	{
		free_all(file, NULL);
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	//5------malloc-----------------------------
	if (!(draw = (char *)malloc(sizeof(char) \
	* list.wcanv * list.hcanv)))
	{
		free_all(file, NULL);
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	//------------------draw----------------------------------------------
	i = -1;
	while (++i < list.wcanv * list.hcanv)
		draw[i] = list.bg;
	//-------------------рисуем прямоугольник-------------------------------------------
	if (!(drawing(file, draw, list)))
	{
		free_all(file, draw);
		write(1, "Error: Operation file corupted\n", 31);
		return (1);
	}
	//-------------------печать----------------------------------------------------------
	//(draw)[(y * list.wcanv) + x] = list.ch;
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
