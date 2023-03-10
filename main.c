// Cristea Marius-Cristian, 313CB
#include "header.h"

int main()
{
	directory *root = malloc(sizeof(directory)); // cream directorul root
	root->pd = NULL;
	root->std = NULL;
	root->drd = NULL;
	root->files = NULL;
	root->directories = NULL;
	root->name = strdup("root");

	directory *d_cr = root; // directorul curent

	char *line = malloc(256);
	int *found = malloc(sizeof(int));
	*found = 0;
	char *command, *parameters;
	while (*(fgets(line, 256, stdin)) != '\n')
	{
		command = strtok(line, " \n");

		if (strcmp(command, "touch") == 0)
		{
			parameters = strtok(NULL, " \n");
			touch(parameters, &d_cr);
		}

		if (strcmp(command, "mkdir") == 0)
		{
			parameters = strtok(NULL, " \n");
			mkdir(parameters, &d_cr);
		}

		if (strcmp(command, "ls") == 0)
			ls(d_cr);

		if (strcmp(command, "rmdir") == 0)
		{
			parameters = strtok(NULL, " \n");
			d_cr->directories = rmdir(d_cr->directories, parameters, found);
			if (*found == 0)
				fprintf(stdout, "Directory %s doesn't exist!\n", parameters);
			else
				*found = 0;
		}

		if (strcmp(command, "rm") == 0)
		{
			parameters = strtok(NULL, " \n");
			d_cr->files = rm(d_cr->files, parameters, found);
			if (*found == 0)
				fprintf(stdout, "File %s doesn't exist!\n", parameters);
			else
				*found = 0;
		}
		if (strcmp(command, "cd") == 0)
		{
			parameters = strtok(NULL, " \n");
			d_cr = cd(d_cr, parameters);
		}
		if (strcmp(command, "pwd") == 0)
		{
			int i = 0;
			char **strings_M = (char**)malloc(20 * sizeof(char*));

			for (i = 0; i < 20; i++)
				strings_M[i] = (char*)malloc(50 * sizeof(char));

			pwd(d_cr, strings_M);

			for (i = 0; i < 20; i++)
				free(strings_M[i]);
			
			free(strings_M);
		}
		
		if (strcmp(command, "find") == 0)
		{
			parameters = strtok(NULL, " \n");
			int i = 0;
			char **strings_M = (char**)malloc(20 * sizeof(char*));

			for (i = 0; i < 20; i++)
				strings_M[i] = (char*)malloc(50 * sizeof(char));


			if (strcmp(parameters, "-d") == 0)
			{
				parameters = strtok(NULL, " \n");
				cauta(root, parameters, found, strings_M);
				if (*found == 0)
					fprintf(stdout, "Directory %s not found!\n", parameters);
				else
					*found = 0;
			}

			if (strcmp(parameters, "-f") == 0)
			{
				parameters = strtok(NULL, " \n");
				cautaFF(root, parameters, found, strings_M);
				if (*found == 0)
					fprintf(stdout, "File %s not found!\n", parameters);
				else
					*found = 0;
			
			}

			for (i = 0; i < 20; i++)
				free(strings_M[i]);
			
			free(strings_M);

		}
		if (strcmp(command, "quit") == 0)
		{
			DistrD(root);
			root = NULL;
			free(found);
			free(line);
			return 0;
		}
	}
	return 0;
}
