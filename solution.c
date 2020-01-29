
/**********************

Progetto facoltativo del corso di Fondamenti di Informatica 2019-2020
Per la descrizione delle strutture e dei prototipi si rimanda al testo della consegna

**********************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{

	int x, y;

} posizione;

typedef struct
{

	posizione pos;
	char ch;
	bool visitato;

} cella;

typedef struct nodo
{
	posizione pos;
	struct nodo *next;
} nodo;

cella *leggi_griglia(char *filename, int *righe, int *colonne);
void stampa_griglia(cella *griglia, int righe, int colonne, posizione pos);
nodo *ricerca(cella *griglia, posizione pos, posizione goal, int righe, int colonne);
bool isEmpty(nodo *lista);
nodo *push_lista(nodo *lista, posizione pos);
int get_cell_grid_number(cella *griglia, int x, int y, int total_elements);
void print_result(nodo *head, cella *griglia, int righe, int colonne);
bool isArrived(cella *griglia, int cell1, int cell2, int cell3, int cell4, posizione goal);
void freeList(nodo *lista);
#define s_buff 22
int main(int argc, char *argv[])
{
	int righe;
	int colonne;
	nodo *lista;
	posizione pos = {1, 1};
	posizione goal = {18, 19};
	cella *griglia = leggi_griglia(argv[1], &righe, &colonne);
	stampa_griglia(griglia, righe, colonne, pos);
	printf("\n-----------------------------\n");
	lista = ricerca(griglia, pos, goal, righe, colonne);
	print_result(lista, griglia, righe, colonne);
	free(griglia);
	freeList(lista);
}

void freeList(nodo *lista)
{
	nodo *temp;
	while (lista != NULL)
	{
		temp = lista;
		lista = lista->next;
		free(temp);
	}
}

void print_result(nodo *head, cella *griglia, int righe, int colonne)
{
	nodo *current_node = head;
	while (current_node != NULL)
	{
		stampa_griglia(griglia, righe, colonne, current_node->pos);
		printf("\n-----------------------------\n\n");
		current_node = current_node->next;
	}
}

int get_cell_grid_number(cella *griglia, int x, int y, int total_elements)
{
	for (int i = 0; i < total_elements; i++)
	{
		if (griglia[i].pos.x == x && griglia[i].pos.y == y)
			return i;
	}
	return 0;
}
nodo *push_lista(nodo *lista, posizione pos)
{
	nodo *temp_node = malloc(sizeof(nodo));
	if (temp_node == NULL)
	{
		printf("Errore nella creazione del nodo.\n");
		exit(-1);
	}
	temp_node->pos = pos;
	temp_node->next = lista;
	return temp_node;
}

bool isArrived(cella *griglia, int cell1, int cell2, int cell3, int cell4, posizione goal)
{
	int temp_arr[4] = {cell1, cell2, cell3, cell4};
	for (int i = 0; i < 4; i++)
	{

		if (griglia[temp_arr[i]].pos.x == goal.x && griglia[temp_arr[i]].pos.y == goal.y)
		{
			return true;
		}
	}
	return false;
}

nodo *ricerca(cella *griglia, posizione pos, posizione goal, int righe, int colonne)
{
	nodo *temp_node;

	int cell_current = get_cell_grid_number(griglia, pos.x, pos.y, righe * colonne);
	int cell_up = get_cell_grid_number(griglia, pos.x, pos.y - 1, righe * colonne);
	int cell_down = get_cell_grid_number(griglia, pos.x, pos.y + 1, righe * colonne);
	int cell_left = get_cell_grid_number(griglia, pos.x - 1, pos.y, righe * colonne);
	int cell_right = get_cell_grid_number(griglia, pos.x + 1, pos.y, righe * colonne);
	if (isArrived(griglia, cell_right, cell_up, cell_down, cell_left, goal))
	{
		temp_node = malloc(sizeof(nodo));
		temp_node->pos = goal;
		temp_node->next = NULL;
		return temp_node;
	}
	else
	{
		if (griglia[cell_current].ch == 'X' || griglia[cell_current].visitato)
		{
			return NULL;
		}
		else
		{

			if (griglia[cell_up].ch == ' ' && !griglia[cell_up].visitato)
			{
				griglia[cell_current].visitato = true;
				temp_node = ricerca(griglia, griglia[cell_up].pos, goal, righe, colonne);

				if (temp_node != NULL)
				{
					return push_lista(temp_node, griglia[cell_up].pos);
				}
			}
			if (griglia[cell_down].ch == ' ' && !griglia[cell_down].visitato)
			{
				griglia[cell_current].visitato = true;
				temp_node = ricerca(griglia, griglia[cell_down].pos, goal, righe, colonne);
				if (temp_node != NULL)
				{
					return push_lista(temp_node, griglia[cell_down].pos);
				}
			}
			if (griglia[cell_left].ch == ' ' && !griglia[cell_left].visitato)
			{
				griglia[cell_current].visitato = true;
				temp_node = ricerca(griglia, griglia[cell_left].pos, goal, righe, colonne);

				if (temp_node != NULL)
				{
					return push_lista(temp_node, griglia[cell_left].pos);
				}
			}
			if (griglia[cell_right].ch == ' ' && !griglia[cell_right].visitato)
			{
				griglia[cell_current].visitato = true;
				temp_node = ricerca(griglia, griglia[cell_right].pos, goal, righe, colonne);

				if (temp_node != NULL)
				{
					return push_lista(temp_node, griglia[cell_right].pos);
				}
			}
			return NULL;
		}
	}
}

cella *leggi_griglia(char *filename, int *righe, int *colonne)
{
	FILE *filePtr;
	filePtr = fopen(filename, "r");
	if (filePtr == NULL)
	{
		printf("Impossibile aprire il file");
		exit(-1);
	}
	cella *cell;
	char buffer[s_buff];
	int num = -2;
	int y = 0;
	while (fgets(buffer, 22, filePtr) != NULL)
	{
		if (num == -2)
		{
			*righe = atoi(buffer);
			num++;
		}
		else if (num == -1)
		{
			*colonne = atoi(buffer);
			num++;
			cell = malloc(sizeof(cella) * (*righe) * (*colonne));
		}
		else
		{
			for (int x = 0; x < *colonne; x++)
			{
				cell[num].pos.x = x;
				cell[num].pos.y = y;
				cell[num].ch = buffer[x];
				cell[num].visitato = false;
				num++;
			}
			y++;
		}
	}
	fclose(filePtr);
	return cell;
}

void stampa_griglia(cella *griglia, int righe, int colonne, posizione pos)
{
	int cell_num = 0;
	for (int i = 0; i < colonne; i++)
	{
		for (int j = 0; j < righe; j++)
		{
			if (i == pos.y && j == pos.x)
				printf("*");
			else
				printf("%c", griglia[cell_num].ch);

			cell_num++;
		}
		printf("\n");
	}
}