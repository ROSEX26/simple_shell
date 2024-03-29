#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

char *_getenv(const char *env_var)
{
	extern char **environ;
	int i = 0;
	char *key;

	while (environ[i])
	{
		key = strtok(environ[i], "=");
		if (strcmp(env_var, key) == 0)
			return (strtok(NULL, "\n"));
		i++;
	}
	return (NULL);
}

char *get_command(char *command)
{
	char *path = _getenv("PATH");
	char *token;
	char *cmd_full;
	struct stat st;

	token = strtok(path, ":");
	while (token)
	{
		cmd_full = malloc(strlen(token) + strlen(command) + 2);
		strcpy(cmd_full, token);
		strcat(cmd_full, "/");
		strcat(cmd_full, command);
		if (stat(cmd_full, &st) == 0)
			return (cmd_full);
		free(cmd_full);
		token = strtok(NULL, ":");
	}
	return (NULL);
}

char **split_string(char *buffer, char *del)
{
	char **tokens;
	char *token;
	int i = 0;

	tokens = malloc(sizeof(char *) * 1024);
	token = strtok(buffer, del);
	while (token)
	{
		tokens[i] = token;
		token = strtok(NULL, del);
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}

int main(int ac, char **av, char **env)
{
	char *buffre = NULL;
	size_t buffer_size = 0;
	char *cmd;
	char **args;
	pid_t pid;
	int status, n_chars;

	while (1)
	{
		write(1, "$ ", 2);
		n_chars = getline(&buffer, &buffer_size, stdin);
		if (n_chars == -1)
		{
			write(1, "\n", 1);
			exit(1);
		}
		args = split_sting(buffer, " \t\n");
		if (strcmp(args[0], "exit") == 0)
			exit(0);
		pid = fork();
		if (pid == 0)
		{
			cmd = get_command(args[0]);
			if (cmd)
				execve(cmd, args, env);
			else 
				printf("command not found\n");
			exit(0);
		}
		else 
			wait(&status);
	}
	return (0);
}
