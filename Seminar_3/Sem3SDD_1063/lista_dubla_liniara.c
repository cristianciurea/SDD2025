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
	struct nodLD* next, *prev; // 2 pointeri de legatura 4B x 2 = 8B
} nodLD;

nodLD* initializareNod(student s)
{
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	//nou->info = s; //shallow copy
	nou->info.varsta = (int*)malloc(sizeof(int));
	*(nou->info.varsta) = *(s.varsta);
	nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, s.nume);
	nou->info.medie = s.medie;
	nou->next = NULL;
	nou->prev = NULL;
	return nou;
}

nodLD* inserareNod1(nodLD* capLD, nodLD **coadaLD, student s)
{
	nodLD* nou = initializareNod(s);
	if (capLD == NULL)
	{
		capLD = nou;
		*coadaLD = nou;
	}
	else
	{
		//nodLD* temp = capLD;
		//while (temp->next != NULL)
		//	temp = temp->next;

		//temp->next = nou; //legatura intre ultimul nod nenul si nodul nou adaugat
		//nou->prev = temp;
		//*coadaLD = nou;

		(*coadaLD)->next = nou;
		nou->prev = *coadaLD;
		*coadaLD = nou;
	}
	return capLD;
}

void inserareNod2(nodLD** capLD, nodLD **coadaLD, student s)
{
	nodLD* nou = initializareNod(s);
	if (*capLD == NULL)
	{
		*capLD = nou;
		*coadaLD = nou;
	}
	else
	{
		nodLD* temp = *capLD;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou; //legatura intre ultimul nod nenul si nodul nou adaugat
		nou->prev = temp;
		*coadaLD = nou;
	}
}

void traversareLista(nodLD* capLD)
{
	nodLD* temp = capLD;
	while (temp != NULL)
	{
		printf("\nVarsta = %d, Nume = %s, Medie = %5.2f",
			*(temp->info.varsta), temp->info.nume, temp->info.medie);
		temp = temp->next;
	}
}

void traversareListaInvers(nodLD* coadaLD)
{
	nodLD* temp = coadaLD;
	while (temp != NULL)
	{
		printf("\nVarsta = %d, Nume = %s, Medie = %5.2f",
			*(temp->info.varsta), temp->info.nume, temp->info.medie);
		temp = temp->prev;
	}
}

void dezalocareLista(nodLD* capLD)
{
	nodLD* temp = capLD;
	while (temp != NULL)
	{
		nodLD* aux = temp->next;
		free(temp->info.varsta);
		free(temp->info.nume);
		free(temp);
		temp = aux;
	}
}

void salvareStudentiVector(nodLD* capLD, student* vect, int* nrElem, float mediePrag)
{
	nodLD* temp = capLD;
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

void stergereStudentDupaNume(nodLD** capLD, nodLD** coadaLD, char* nume)
{
	if (strcmp((*capLD)->info.nume, nume) == 0)
	{
		nodLD* temp = *capLD;
		*capLD = (*capLD)->next;
		if (*capLD != NULL)
			(*capLD)->prev = NULL;
		free(temp->info.varsta);
		free(temp->info.nume);
		free(temp);
		return;
	}
	else
		if (strcmp((*coadaLD)->info.nume, nume) == 0)
		{
			nodLD* temp = *coadaLD;
			*coadaLD = (*coadaLD)->prev;
			if (*coadaLD != NULL)
				(*coadaLD)->next = NULL;
			free(temp->info.varsta);
			free(temp->info.nume);
			free(temp);
			return;
		}
		else
		{
			nodLD* temp = *capLD;
			while (temp != NULL)
			{
				if (strcmp(temp->info.nume, nume) == 0)
				{
					nodLD* anterior = temp->prev;
					nodLD* urmator = temp->next;
					anterior->next = urmator;
					urmator->prev = anterior;
					free(temp->info.varsta);
					free(temp->info.nume);
					free(temp);
					return;
				}
				else
					temp = temp->next;
			}
		}
}

void main()
{
	int nrStud;
	student s;
	nodLD* capLD = NULL, *coadaLD = NULL;
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
		capLD = inserareNod1(capLD, &coadaLD, s);
		//inserareNod2(&capLD, &coadaLD, s);
		free(s.nume);
		free(s.varsta);
	}
	fclose(f);

	traversareLista(capLD);
	printf("\n-------------Traversare inversa--------------\n");
	traversareListaInvers(coadaLD);

	//salvare vector
	printf("\n-------------Vector--------------\n");
	student* vect = (student*)malloc(nrStud * sizeof(student));
	int nrElem = 0;
	salvareStudentiVector(capLD, vect, &nrElem, 9.0f);
	for (int i = 0; i < nrElem; i++)
		printf("\nVarsta = %d, Nume = %s, Medie = %5.2f",
			*(vect[i].varsta), vect[i].nume, vect[i].medie);
	for (int i = 0; i < nrElem; i++)
	{
		free(vect[i].varsta);
		free(vect[i].nume);
	}
	free(vect);

	printf("\n-------------Stergere--------------\n");
	stergereStudentDupaNume(&capLD, &coadaLD, "Vasilica");
	traversareLista(capLD);
	printf("\n-------------Traversare inversa--------------\n");
	traversareListaInvers(coadaLD);

	dezalocareLista(capLD);
}