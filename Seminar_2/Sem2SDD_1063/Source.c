#include <stdio.h>
#include <malloc.h>

typedef struct
{
	int* varsta; //4B
	char* nume; //4B
	float medie; //4B
} student;

typedef struct
{
	student info; //informatie utila
	struct nodLS* next; //pointer de legatura 4B
} nodLS;

nodLS* initializareNod(student s)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	//nou->info = s; //shallow copy
	nou->info.varsta = (int*)malloc(sizeof(int));
	*(nou->info.varsta) = *(s.varsta);
	nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, s.nume);
	nou->info.medie = s.medie;
	nou->next = NULL;
	return nou;
}

nodLS* inserareNod1(nodLS* capLS, student s)
{
	nodLS* nou = initializareNod(s);
	if (capLS == NULL)
		capLS = nou;
	else
	{
		nodLS* temp = capLS;
		while (temp->next != NULL)
			temp = temp->next;

		temp->next = nou; //legatura intre ultimul nod nenul si nodul nou adaugat
	}
	return capLS;
}

void inserareNod2(nodLS** capLS, student s)
{
	nodLS* nou = initializareNod(s);
	if (*capLS == NULL)
		*capLS = nou;
	else
	{
		nodLS* temp = *capLS;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou; //legatura intre ultimul nod nenul si nodul nou adaugat
	}
}

void traversareLista(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		printf("\nVarsta = %d, Nume = %s, Medie = %5.2f",
			*(temp->info.varsta), temp->info.nume, temp->info.medie);
		temp = temp->next;
	}
}

void dezalocareLista(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		nodLS* aux = temp->next;
		free(temp->info.varsta);
		free(temp->info.nume);
		free(temp);
		temp = aux;
	}
}

void salvareStudentiVector(nodLS* capLS, student* vect, int* nrElem, float mediePrag)
{
	nodLS* temp = capLS;
	while (temp != NULL)
	{
		if (temp->info.medie >= mediePrag)
		{
			//vect[*nrElem] = temp->info;//shallow copy
			vect[*nrElem].varsta = (int*)malloc(sizeof(int));
			*(vect[*nrElem].varsta) = *(temp->info.varsta);
			vect[*nrElem].nume = (char*)malloc((strlen(temp->info.nume) + 1) *
				sizeof(char));
			strcpy(vect[*nrElem].nume, temp->info.nume);
			vect[*nrElem].medie = temp->info.medie;
			(*nrElem)++;
		}
		temp = temp->next;
	}
}

void main()
{
	int nrStud;
	student s;
	nodLS* capLS = NULL;
	char buffer[20];
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nrStud);
	for (int i = 0; i < nrStud; i++)
	{
		s.varsta = (int*)malloc(sizeof(int));
		fscanf(f, "%d", s.varsta);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%f", &s.medie);
		//capLS = inserareNod1(capLS, s);
		inserareNod2(&capLS, s);
		free(s.nume);
		free(s.varsta);
	}
	fclose(f);

	traversareLista(capLS);
	
	//salvare vector
	printf("\n-------------Vector--------------\n");
	student* vect = (student*)malloc(nrStud * sizeof(student));
	int nrElem = 0;
	salvareStudentiVector(capLS, vect, &nrElem, 9.0f);
	for (int i = 0; i < nrElem; i++)
		printf("\nVarsta = %d, Nume = %s, Medie = %5.2f",
			*(vect[i].varsta), vect[i].nume, vect[i].medie);
	for (int i = 0; i < nrElem; i++)
	{
		free(vect[i].varsta);
		free(vect[i].nume);
	}
	free(vect);

	dezalocareLista(capLS);
}


