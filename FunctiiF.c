// Cristea Marius-Cristian, 313CB
#include "header.h"

// functie de cautare a unui fisier
file *cautaF(file *fl, char *nume)
{
	if (fl == NULL || strcmp(nume, fl->name) == 0)
		return fl;

	if (strcmp(nume, fl->name) > 0)
		return cautaF(fl->drf, nume);

	return cautaF(fl->stf, nume);
}

int touch(char *nume, directory **d_cr)
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

	if ((*d_cr)->files == NULL)
	{
		(*d_cr)->files = (file *)malloc(sizeof(file));
		if (!(*d_cr)->files)
			return -1;

		((file *)(*d_cr)->files)->name = strdup(nume);
		((file *)(*d_cr)->files)->stf = NULL;
        ((file *)(*d_cr)->files)->drf = NULL;
		((file *)(*d_cr)->files)->pf = *d_cr;
		return 1;
	}

	file *aux, *p, *n;
	n = (*d_cr)->files;

	while (n)
	{
		p = n;
		if (strcmp(nume, n->name) > 0)
			n = n->drf;
		else
			n = n->stf;
	}

	aux = (file *)malloc(sizeof(file));
	if (!aux)
		return -1;

	aux->name = strdup(nume);
	aux->stf = NULL;
    aux->drf = NULL;
	aux->pf = *d_cr;
	if (strcmp(nume, p->name) > 0)
		p->drf = aux;
	else
		p->stf = aux;
	return 1;
}

file *rm(file *arb_fl, char *nume, int *verificare)
{
	if (arb_fl == NULL)
		return arb_fl;

	if (strcmp(nume, arb_fl->name) > 0)
		arb_fl->drf = rm(arb_fl->drf, nume, verificare);

	else if (strcmp(nume, arb_fl->name) < 0)
		arb_fl->stf = rm(arb_fl->stf, nume, verificare);

	else
	{
		(*verificare)++;
		if (arb_fl->stf == NULL)
		{
			file *aux = arb_fl->drf;
			free(arb_fl->name);
			arb_fl->pf = NULL;
			free(arb_fl);
			arb_fl = NULL;
			return aux;
		}
		else if (arb_fl->drf == NULL)
		{
			file *aux = arb_fl->stf;
			free(arb_fl->name);
			arb_fl->pf = NULL;
			free(arb_fl);
			arb_fl = NULL;
			return aux;
		}
		file *fmin = minF(arb_fl->drf);
		free(arb_fl->name);
		arb_fl->name = strdup(fmin->name);
		arb_fl->drf = rm(arb_fl->drf, fmin->name, verificare);
	}

	return arb_fl;
}

// functie de cautare a nodului cu nume cel mai mic lexicografic - pentru files
file *minF(file *fl)
{
	file *curr = fl;
	while (curr->stf != NULL)
		curr = curr->stf;

	return curr;
}

// functie de afisare a unui ABC de tip files
void afi_F(file *f)
{
	if (f->stf)
		afi_F(f->stf);

	printf("%s ", f->name);

	if (f->drf)
		afi_F(f->drf);
}

// functia de cautare a unui fisier pentru find -f
void cautaFF(directory *dir, char *nume, int* found, char **M)
{
	if (dir == NULL)
		return;
	
	file *f1 = cautaF(dir->files, nume);
	if (f1)
	{
		(*found)++;
		printf("File %s found!\n", nume);
		pwd(dir, M);
		return;
	}
	cautaFF(dir->directories, nume, found, M);
	cautaFF(dir->std, nume, found, M);
	cautaFF(dir->drd, nume, found, M);
}

// functie de eliberare a unui ABC de tip files
void DistrF(file *f)
{
	if (!f)
		return;
	DistrF(f->stf);
	DistrF(f->drf);
	free(f->name);
	f->pf = NULL;
	free(f);
	f = NULL;
}