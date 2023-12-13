#include "main.h"

/**
 * execcmd - execute cmd,
 * @argv: the cmd to be executed
 */
int execcmd(char **cmd, char *errormsg)
{
	int execode;

	execode = execve(cmd[0], cmd, NULL);
	_perror(errormsg);

	return (execode);
}

int shell_int(char **line, size_t *len, char *pname)
{
	char *path, **toks;
	int (*btin)(char **, char *);
	pid_t child_pid;
	struct stat buf;

	while (1)
	{
		_printf("($) ");
		readcmd(line, len);
		if (*line[0] != '\n')
		{
			toks = _strtok(*line, " \t\n");
			/*free(*line);*/
			btin = is_btin(toks[0]);

			if (btin)
			{
				btin(toks, pname);
				/*free_toks(toks);*/
			}
			else
			{
				path = findxpath(toks[0]);
				free(toks[0]);
				toks[0] = path;

				readx(toks, pname, buf, &child_pid);
			}
		}
		/*free(*line);*/
		free_toks(toks);
	}

	return (0);
}

int shelln_int(char **line, size_t *len, char *pname)
{
	char *path, **toks;
	int execres, (*btin)(char **, char *);
	pid_t child_pid;
	struct stat buf;

	readcmd(line, len);
	toks = _strtok(*line, " \t\n");
	/*free(*line);*/

	btin = is_btin(toks[0]);

	if (btin)
	{
		execres = btin(toks, pname);
	}
	else
	{
		path = findxpath(toks[0]);
		free(toks[0]);
		toks[0] = path;

		execres = readx(toks, pname, buf, &child_pid);
	}

	free_toks(toks);

	return (execres);
}

int readx(char **toks, char *pname, struct stat buf, pid_t *child_pid)
{
	if (stat(toks[0], &buf) == 0)
	{
		*child_pid = fork();
		if (*child_pid == -1)
		{
			_perror("Error:");
			/*free_toks(toks);*/
			return (-1);
		}
		else if (*child_pid == 0)
		{
			execcmd(toks, pname);
		}
		else
		{
			wait(NULL);
			/*free_toks(toks);*/
		}
	} else {
		_perror(pname);
		/*free_toks(toks);*/
		return (-1);
	}

	return (0);
}
