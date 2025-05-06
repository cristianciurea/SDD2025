#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int cod;
	char* titlu;
	int nrAutori;
	float pret;
} carte;

typedef struct
{
	carte inf; //16B
	struct nodArb* stanga, * dreapta; //8B
} nodArb;

nodArb* creareNod(carte c, nodArb* st, nodArb* dr)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->inf.cod = c.cod;
	nou->inf.titlu = (char*)malloc((strlen(c.titlu) + 1) * sizeof(char));
	strcpy(nou->inf.titlu, c.titlu);
	nou->inf.nrAutori = c.nrAutori;
	nou->inf.pret = c.pret;
	nou->stanga = st;
	nou->dreapta = dr;
	return nou;
}

nodArb* inserareNod(nodArb* rad, carte c)
{
	if (rad != NULL)
	{
		if (c.cod < rad->inf.cod)
		{
			rad->stanga = inserareNod(rad->stanga, c);
			return rad;
		}
		else
			if (c.cod > rad->inf.cod)
			{
				rad->dreapta = inserareNod(rad->dreapta, c);
				return rad;
			}
			else
				return rad;
	}
	else
		return creareNod(c, NULL, NULL);
}

//RSD
void preordine(nodArb* rad)
{
	if (rad != NULL)
	{
		printf("\nCod=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f",
			rad->inf.cod, rad->inf.titlu, rad->inf.nrAutori, rad->inf.pret);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

//SRD
void inordine(nodArb* rad)
{
	if (rad != NULL)
	{
		inordine(rad->stanga);
		printf("\nCod=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f",
			rad->inf.cod, rad->inf.titlu, rad->inf.nrAutori, rad->inf.pret);
		inordine(rad->dreapta);
	}
}

//SDR
void postordine(nodArb* rad)
{
	if (rad != NULL)
	{
		postordine(rad->stanga);
		postordine(rad->dreapta);
		printf("\nCod=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f",
			rad->inf.cod, rad->inf.titlu, rad->inf.nrAutori, rad->inf.pret);
	}
}

void dezalocare(nodArb* rad)
{
	//preordine
	//if (rad != NULL)
	//{
	//	nodArb* st = rad->stanga;
	//	nodArb* dr = rad->dreapta;
	//	free(rad->inf.titlu);
	//	free(rad);
	//	dezalocare(st);
	//	dezalocare(dr);
	//}
	//postordine
	if (rad != NULL)
	{
		dezalocare(rad->stanga);
		dezalocare(rad->dreapta);
		free(rad->inf.titlu);
		free(rad);
	}
}

void numarCartiAutori(nodArb* rad, int* nr, int prag)
{
	if (rad != NULL)
	{
		if (rad->inf.nrAutori > prag)
			(*nr)++;
		numarCartiAutori(rad->stanga, nr, prag);
		numarCartiAutori(rad->dreapta, nr, prag);
	}
}

void salvareCartiVector(nodArb* rad, carte* vect, int* nr, float pretPrag)
{
	if (rad != NULL)
	{
		if (rad->stanga==NULL && rad->dreapta==NULL && rad->inf.pret > pretPrag)
		{
			vect[*nr].cod = rad->inf.cod;
			vect[*nr].titlu = (char*)malloc((strlen(rad->inf.titlu) + 1) * sizeof(char));
			strcpy(vect[*nr].titlu, rad->inf.titlu);
			vect[*nr].nrAutori = rad->inf.nrAutori;
			vect[*nr].pret = rad->inf.pret;
			(*nr)++;
		}
		salvareCartiVector(rad->stanga, vect, nr, pretPrag);
		salvareCartiVector(rad->dreapta, vect, nr, pretPrag);
	}
}

int maxim(int a, int b)
{
	int max = a;
	if (max < b)
		max = b;
	return max;
}

int inaltimeArbore(nodArb* rad)
{
	if (rad != NULL)
		return 1 + maxim(inaltimeArbore(rad->stanga), 
			inaltimeArbore(rad->dreapta));
	else
		return 0;
}

void main()
{
	nodArb* rad = NULL;
	carte c;
	int nrCarti;
	char buffer[30];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrCarti);
	for (int i = 0; i < nrCarti; i++)
	{
		fscanf(f, "%d", &c.cod);
		fscanf(f, "%s", buffer);
		c.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(c.titlu, buffer);
		fscanf(f, "%d", &c.nrAutori);
		fscanf(f, "%f", &c.pret);
		rad = inserareNod(rad, c);
		free(c.titlu);
	}
	fclose(f);

	printf("\n-------preordine----\n");
	preordine(rad);
	printf("\n-------inordine----\n");
	inordine(rad);
	printf("\n-------postordine----\n");
	postordine(rad);

	printf("\n----------------------\n");
	int nr = 0;
	numarCartiAutori(rad, &nr, 2);
	printf("\nNumar carti autori = %d", nr);

	printf("\n----------------------\n");
	carte* vect = (carte*)malloc(nrCarti * sizeof(carte));
	int nrC = 0;
	salvareCartiVector(rad, vect, &nrC, 110);
	for (int i = 0; i < nrC; i++)
		printf("\nCod=%d, Titlu=%s, Nr. autori=%d, Pret=%5.2f",
			vect[i].cod, vect[i].titlu, vect[i].nrAutori, vect[i].pret);

	for (int i = 0; i < nrC; i++)
		free(vect[i].titlu);
	free(vect);

	printf("\n----------------------\n");
	printf("\nInaltime arbore = %d", inaltimeArbore(rad));

	dezalocare(rad);
}