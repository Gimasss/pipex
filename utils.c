/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giomastr <giomastr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:21:55 by giomastr          #+#    #+#             */
/*   Updated: 2025/05/06 17:55:38 by giomastr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_fd(t_pipex *pip)
{
	close(pip->pipefd[0]);
	close(pip->pipefd[1]);
	close(pip->infile_fd);
	close(pip->outfile_fd);
}

void	error_exit(char *s, t_pipex *pip)
{
	perror(s);
	close_fd(pip);
	if (pip->cmd1)
		free_matrix(pip->cmd1);
	if (pip->cmd1)
		free_matrix(pip->cmd2);
	close(0);
	close(1);
	exit(EXIT_FAILURE);
}
void	free_matrix(char **arr)
{
	int i = 0;
	while (arr && arr[i])
		free(arr[i++]);
	free(arr);
}

char	*get_full_path(char *path, char *cmd)
{
	char	*full_path;
	size_t	tot_len;
	size_t	path_len;

	path_len = ft_strlen(path);
	tot_len = path_len + ft_strlen(cmd) + 2;
	full_path = ft_calloc(sizeof(char), tot_len);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, path, tot_len);
	full_path[path_len] = '/';
	ft_strlcat(full_path, cmd, tot_len);
	return (full_path);
}

char	*cmd_path(char *cmd, char **envp)// Find the full path of the binary using PATH
{
	int		i;
	char	**paths;
	char 	*full_path;

	i = 0;
	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))//if it's direct path ft_strchr('/')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split((envp[i] + 5), ':');
	i = 0;
	while (paths[i])
	{
		full_path = get_full_path(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			return(full_path);
			free(full_path);
		}
		free(full_path);
		i++;
	}
	i = -1;
	free_matrix(paths);
	return (0);
}

//////// TEST CMD_PATH
/* if (argc != 2)
 {
	 fprintf(stderr, "Usage: %s <command>\n", argv[0]);
	 return 1;
 }

 char *cmd = argv[1];  // Command to test
 char *path = cmd_path(cmd, envp);

 if (path)
 {
	 printf("Found command: %s\n", path);
	 free(path);  // Don't forget to free the memory returned by find_cmd_path!
 }
 else
 {
	 printf("Command not found: %s\n", cmd);
 }

 return 0; */



//////// TEST FULL_PATH
/* int main()
{
	char *path = "/usr/bin";
	char *cmd = "grep";
	char *result;

	result = get_full_path(path, cmd);
	if (result == NULL)
	{
		printf("Memory allocation failed.\n");
		return (1);
	}

	printf("Full path: %s\n", result);
	free(result);
	return (0);
} */