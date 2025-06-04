#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int id;
	struct nodStiva* next;
} nodStiva;

typedef struct
{
	int id;
	struct nodCoada* next;
} nodCoada;

void push(nodStiva** varf, int idNod)
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->id = idNod;
	nou->next = NULL;
	if (*varf == NULL)
		*varf = nou;
	else
	{
		nou->next = *varf;
		*varf = nou;
	}
}

int pop(nodStiva** varf, int* idNod)
{
	if (*varf == NULL)
		return -1;
	else
	{
		*idNod = (*varf)->id;
		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp);
		return 0;
	}
}

void put(nodCoada** prim, nodCoada** ultim, int idNod)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->id = idNod;
	nou->next = NULL;
	if (*prim == NULL || *ultim == NULL)
	{
		*prim = nou;
		*ultim = nou;
	}
	else
	{
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

int get(nodCoada** prim, nodCoada** ultim, int* idNod)
{
	if (*prim != NULL && *ultim != NULL)
	{
		*idNod = (*prim)->id;
		nodCoada* temp = *prim;
		*prim = (*prim)->next;
		free(temp);
		return 0;
	}
	if (*prim == NULL)
	{
		*ultim = NULL;
		return -1;
	}
}

void parcurgereAdancime(int** mat, int* vizitate, int nrNoduri, int idNodStart)
{
	for (int i = 0; i < nrNoduri; i++)
		vizitate[i] = 0;

	nodStiva* varf = NULL;

	push(&varf, idNodStart);
	vizitate[idNodStart] = 1;

	while (varf != NULL)
	{
		pop(&varf, &idNodStart);
		printf("%d -> ", idNodStart + 1);
		for(int k=0;k<nrNoduri;k++)
			if (mat[idNodStart][k] != 0 && vizitate[k] == 0)
			{
				push(&varf, k);
				vizitate[k] = 1;
			}
	}
}

void parcurgereLatime(int** mat, int* vizitate, int nrNoduri, int idNodStart)
{
	for (int i = 0; i < nrNoduri; i++)
		vizitate[i] = 0;

	nodCoada* prim = NULL, *ultim = NULL;

	put(&prim, &ultim, idNodStart);
	vizitate[idNodStart] = 1;

	while (prim != NULL)
	{
		get(&prim, &ultim, &idNodStart);
		printf("%d -> ", idNodStart + 1);
		for (int k = 0; k < nrNoduri; k++)
			if (mat[idNodStart][k] != 0 && vizitate[k] == 0)
			{
				put(&prim, &ultim, k);
				vizitate[k] = 1;
			}
	}
}

void main()
{
	FILE* f = fopen("fisier.txt", "r");

	int nrNoduri;
	fscanf(f, "%d", &nrNoduri);

	int** mat = (int**)malloc(nrNoduri * sizeof(int*));
	for (int i = 0; i < nrNoduri; i++)
		mat[i] = (int*)malloc(nrNoduri * sizeof(int));

	for (int i = 0; i < nrNoduri; i++)
		for (int j = 0; j < nrNoduri; j++)
			mat[i][j] = 0;

	int nrArce, idNodStart, idNodStop, greutateArc;
	fscanf(f, "%d", &nrArce);
	for (int i = 0; i < nrArce; i++)
	{
		fscanf(f, "%d", &idNodStart);
		fscanf(f, "%d", &idNodStop);
		fscanf(f, "%d", &greutateArc);

		mat[idNodStart - 1][idNodStop - 1] = greutateArc;
		mat[idNodStop - 1][idNodStart - 1] = greutateArc;
	}

	fclose(f);

	int* vizitate = (int*)malloc(nrNoduri * sizeof(int));

	printf("\nMatrice adiacenta:\n");
	for (int i = 0; i < nrNoduri; i++)
	{
		for (int j = 0; j < nrNoduri; j++)
			printf("%d ", mat[i][j]);
		printf("\n");
	}

	printf("\nParcurgere adancime de la nodul: ");
	scanf("%d", &idNodStart);
	parcurgereAdancime(mat, vizitate, nrNoduri, idNodStart);

	printf("\nParcurgere latime de la nodul: ");
	scanf("%d", &idNodStart);
	parcurgereLatime(mat, vizitate, nrNoduri, idNodStart);
}