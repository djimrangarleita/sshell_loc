#include "main.h"

char **_strtok(char *str, char *delim)
{	
	int i, toksnum = 0;
	char *tok, *dupstr, **toks;

	dupstr = _strdup(str);
	tok = strtok(dupstr, delim);
	while (tok != NULL)
	{
		tok = strtok(NULL, delim);
		toksnum++;
	}
	toks = malloc(sizeof(char *) * toksnum + 1);
	free(dupstr);
	
	tok = strtok(str, delim);
	for (i = 0; i < toksnum; i++)
	{
		toks[i] = malloc(sizeof(char) * _strlen(tok));
		_strcpy(toks[i], tok);
		tok = strtok(NULL, delim);
	}
	toks[i] = NULL;

	return (toks);
}

char *findxpath(char *input)
{
	int i;
	char *key, *val, *path, *tmp, *tmpcat, *pathexec, *tmpenv = NULL;
	struct stat buf;

	if (*input == '/')
	{
		return (input);
	}

	i = 0;
	while (environ[i] != NULL)
	{
		tmpenv = strdup(environ[i]);
		key = strtok(tmpenv, "=");
		val = strtok(NULL, "=");
		if (_strcmp(key, "PATH") == 0)
		{
			path = strtok(val, ":");
			while (path != NULL)
			{
				tmp = strdup(path);
				tmp = _strcat(tmp, "/");
				tmpcat = _strcat(tmp, input);
				if (stat(tmpcat, &buf) == 0)
				{
					pathexec = malloc(_strlen(tmpcat) * sizeof(char));
					pathexec = _strcpy(pathexec, tmpcat);

					return (pathexec);
				}
				path = strtok(NULL, ":");
			}
		}
		i++;
	}

	return (input);
}

void free_toks(char **tokens)
{
	int i = 0;

	while (tokens[i] != NULL)
		free(tokens[i++]);
	free(tokens);
}

int readcmd(char **line, size_t *len)
{
	int rchars;

	rchars = getline(line, len, stdin);
	if (rchars == -1)
	{
		_perror("Exiting shell...");
		free(*line);
		exit(-1);
	}

	return (rchars);
}
