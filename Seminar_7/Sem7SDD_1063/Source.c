#include<stdio.h>
#include<malloc.h>

typedef struct
{
	int prioritate;
	char* denumire;
	float durata;
} activitate;

typedef struct
{
	activitate* vect;
	int nrElem;
} heap;

void filtrare(heap h, int index)
{
	int indexRad = index;
	int indexSt = 2 * index + 1;
	int indexDr = 2 * index + 2;

	if (indexSt<h.nrElem && h.vect[indexSt].prioritate >
		h.vect[indexRad].prioritate)
		indexRad = indexSt;
	if (indexDr<h.nrElem && h.vect[indexDr].prioritate >
		h.vect[indexRad].prioritate)
		indexRad = indexDr;

	if (index != indexRad)
	{
		activitate aux = h.vect[index];
		h.vect[index] = h.vect[indexRad];
		h.vect[indexRad] = aux;

		filtrare(h, indexRad);
	}
}

heap inserare(heap h, activitate noua)
{
	activitate* vectNou = (activitate*)malloc((h.nrElem + 1) * sizeof(activitate));
	for (int i = 0; i < h.nrElem; i++)
		vectNou[i] = h.vect[i];
	h.nrElem++;
	vectNou[h.nrElem - 1] = noua;
	free(h.vect);
	h.vect = vectNou;
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	return h;
}

heap extragere(heap h, activitate* extrasa)
{
	activitate* vectNou = (activitate*)malloc((h.nrElem - 1) * sizeof(activitate));
	activitate aux = h.vect[0];
	h.vect[0] = h.vect[h.nrElem - 1];
	h.vect[h.nrElem - 1] = aux;

	*extrasa = h.vect[h.nrElem - 1];

	h.nrElem--;
	for (int i = 0; i < h.nrElem; i++)
		vectNou[i] = h.vect[i];
	free(h.vect);
	h.vect = vectNou;
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	return h;
}

void afisareHeap(heap h)
{
	for (int i = 0; i < h.nrElem; i++)
		printf("\nPrioritate = %d, Denumire = %s, Durata = %5.2f",
			h.vect[i].prioritate, h.vect[i].denumire,
			h.vect[i].durata);
}

void dezalocareHeap(heap h)
{
	for (int i = 0; i < h.nrElem; i++)
		free(h.vect[i].denumire);
	free(h.vect);
}

void main()
{
	heap h;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &h.nrElem);
	h.vect = (activitate*)malloc(h.nrElem * sizeof(activitate));
	for (int i = 0; i < h.nrElem; i++)
	{
		fscanf(f, "%d", &h.vect[i].prioritate);
		fscanf(f, "%s", buffer);
		h.vect[i].denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(h.vect[i].denumire, buffer);
		fscanf(f, "%f", &h.vect[i].durata);
	}
	fclose(f);
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);

	afisareHeap(h);

	activitate noua;
	printf("Prioritate = ");
	scanf("%d", &noua.prioritate);
	printf("Denumire = ");
	scanf("%s", buffer);
	noua.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(noua.denumire, buffer);
	printf("Durata = ");
	scanf("%f", &noua.durata);

	printf("\n-------------------------------\n");

	h = inserare(h, noua);

	afisareHeap(h);

	printf("\n-------------------------------\n");

	activitate extrasa;
	h = extragere(h, &extrasa);

	afisareHeap(h);

	printf("\nExtrasa -> Prioritate = %d, Denumire = %s, Durata = %5.2f",
		extrasa.prioritate, extrasa.denumire, extrasa.durata);

	dezalocareHeap(h);
}