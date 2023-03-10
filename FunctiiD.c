// Cristea Marius-Cristian, 313CB
#include "header.h"

// functie de cautare a unui director
directory *cautaDir(directory *dir, char *nume)
{
	if (dir == NULL || strcmp(nume, dir->name) == 0)
		return dir;

	if (strcmp(nume, dir->name) > 0)
		return cautaDir(dir->drd, nume);

	return cautaDir(dir->std, nume);
}

int mkdir(char *nume, directory **d_cr)
{

	directory *cautaD = cautaDir((*d_cr)->directories, nume);
	if (cautaD)
	{
		printf("Directory %s already exists!\n", nume);
		return 0;
	}
	file *cautaFi = cautaF((*d_cr)->files, nume);
	if (cautaFi)
	{
		printf("File %s already exists!\n", nume);
		return 0;
	}

	if ((*d_cr)->directories == NULL)
	{
		// daca nu exista niciun subdirector
		(*d_cr)->directories = (directory *)malloc(sizeof(directory));
		if (!(*d_cr)->directories)
			return -1;
		((directory *)(*d_cr)->directories)->name = strdup(nume);
		((directory *)(*d_cr)->directories)->std = NULL;
		((directory *)(*d_cr)->directories)->drd = NULL;
		((directory *)(*d_cr)->directories)->files = NULL;
		((directory *)(*d_cr)->directories)->directories = NULL;
		((directory *)(*d_cr)->directories)->pd = *d_cr;
		return 1;
	}
	directory *aux, *p, *n;
	n = (*d_cr)->directories;
	while (n)
	{
		p = n;
		if (strcmp(nume, n->name) > 0)
			n = n->drd;
		else
			n = n->std;
	}

	aux = (directory *)malloc(sizeof(directory));
	if (!aux)
		return -1;

	aux->name = strdup(nume);
	if (!aux->name)
	{
		free(aux);
		return -1;
	}
	aux->std = NULL;
    aux->drd = NULL;
	aux->pd = *d_cr;
	aux->directories = NULL;
	aux->files = NULL;
	if (strcmp(nume, p->name) > 0)
		p->drd = aux;
	else
		p->std = aux;

	return 1;
}

directory *rmdir(directory *arb_dir, char *nume, int *verificare)
{
	if (arb_dir == NULL)
		return arb_dir;

	if (strcmp(nume, arb_dir->name) > 0)
		arb_dir->drd = rmdir(arb_dir->drd, nume, verificare);

	else if (strcmp(nume, arb_dir->name) < 0)
		arb_dir->std = rmdir(arb_dir->std, nume, verificare);

	else
	{
		(*verificare)++;
		if (arb_dir->std == NULL)
		{
			directory *aux = arb_dir->drd;
			DistrD(arb_dir->directories);
			DistrF(arb_dir->files);
			free(arb_dir->name);
			free(arb_dir);
			return aux;
		}
		else if (arb_dir->drd == NULL)
		{
			directory *aux = arb_dir->std;
			DistrD(arb_dir->directories);
			DistrF(arb_dir->files);
			free(arb_dir->name);
			free(arb_dir);
			return aux;
		}
		directory *dmin = minD(arb_dir->drd);
		free(arb_dir->name);
		arb_dir->name = strdup(dmin->name);
		arb_dir->drd = rmdir(arb_dir->drd, dmin->name, verificare);
	}
	return arb_dir;
}

// functie de cautare a nodului cu nume cel mai mic lexicografic
directory *minD(directory *dr)
{
	directory *curr = dr;
	while (curr->std != NULL)
		curr = curr->std;

	return curr;
}

// functie de afisare a unui ABC cu directoare
void afi_D(directory *d)
{
	if (d->std)
		afi_D(d->std);

	printf("%s ", d->name);

	if (d->drd)
		afi_D(d->drd);
}

int ls(directory *d_cr)
{
	int countd = 0;
	int countf = 0;

	if (d_cr->directories == NULL)
		countd++;

	if (d_cr->files == NULL)
		countf++;

	if (countd == 1 && countf == 1)
	{
		printf("\n");
		return 0;
	}
	else if (countd == 1)
	{
		afi_F(d_cr->files);
		printf("\n");
	}
	else if (countf == 1)
	{
		afi_D(d_cr->directories);
		printf("\n");
	}
	else
	{
		afi_D(d_cr->directories);
		afi_F(d_cr->files);
		printf("\n");
	}

	return 1;
}

directory *cd(directory *d_cr, char *nume)
{
	if (strcmp(nume, "..") == 0)
	{
		if (strcmp(d_cr->name, "root") == 0)
		{
			// root nu are director parinte
			return d_cr;
		}
		d_cr = d_cr->pd;
		return d_cr;
	}

	directory *new = cautaDir(d_cr->directories, nume);
	if (new == NULL)
	{
		// daca nu gasim directorul
		printf("Directory not found!\n");
		return d_cr;
	}
	else
	{
		d_cr = new;
		return d_cr;
	}
}

void pwd(directory *dir, char **M)
{
	int i = 0, j = 0;
	while (dir->pd)
	{
		strcpy(M[i], dir->name);
		i++;
		dir = dir->pd;
	}
	printf("/root");
	for (j = i-1; j >= 0; j--)
	{
		printf("/%s", M[j]);
		dir = cd(dir, M[j]);
	}
	printf("\n");
	return;
}

// functie de cautare a unui director pentru find -d
void cauta(directory *dir, char *nume, int *found, char **M)
{
	if (dir == NULL)
		return;
	if (strcmp(nume, dir->name) == 0)
	{
		(*found)++;
		printf("Directory %s found!\n", nume);
		pwd(dir, M);
		return;
	}
	cauta(dir->directories, nume, found, M);
	cauta(dir->std, nume, found, M);
	cauta(dir->drd, nume, found, M);
}

// functie de eliberare a unui ABC de tip director
void DistrD(directory *d)
{
	if (!d)
		return;
	DistrD(d->directories);
	DistrF(d->files);
	DistrD(d->std);
	DistrD(d->drd);
	free(d->name);
	d->pd = NULL;
	d->std = NULL;
	d->drd = NULL;
	free(d);
	d = NULL;
}
