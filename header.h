// Cristea Marius-Cristian, 313CB
/* header.h */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifndef DandF
#define DandF

typedef struct file {
	char *name;
	struct file *stf, *drf; 
	struct directory *pf;
} file;

typedef struct directory {
	char *name;
	struct directory *std, *drd, *pd;
	struct file *files;
	struct directory *directories;
} directory;

directory *cautaDir(directory * dir, char *nume);
int mkdir(char *nume, directory **d_cr);
directory *minD(directory *dr);
directory *cd(directory *d_cr, char *nume);
void afi_D(directory *d);
int ls(directory *d_cr);
file *cautaF(file *fl, char *nume);
int touch(char *nume, directory **d_cr);
void afi_F(file *f);
file *minF(file *fl);
file *rm(file *arb_fl, char *nume, int *verificare);
void DistrF(file *f);
void DistrD(directory *d);
directory *rmdir(directory *arb_dir, char *nume, int *verificare);
void cauta(directory *dir, char *nume, int *found, char **M);
void cautaFF(directory *dir, char *nume, int* foun, char **M);
void pwd(directory *dir, char **M);
#endif
