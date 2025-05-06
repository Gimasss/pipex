/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giomastr <giomastr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:21:58 by giomastr          #+#    #+#             */
/*   Updated: 2025/05/06 18:47:26 by giomastr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <bits/fcntl-linux.h>

void	init_data(t_pipex *pip, char **argv)
{
	*pip = (t_pipex){0};
	pip->cmd1 = ft_split(argv[2], ' ');
	if (!pip->cmd1)
		error_exit("cmd1 allocation failed", pip);
	pip->cmd2 = ft_split(argv[3], ' ');
	if (!pip->cmd2)
		error_exit("cmd2 allocation failed", pip);
	pip->infile_fd = open(argv[1], O_RDONLY);
	if (pip->infile_fd == -1)
		error_exit("infile open error", pip);
	pip->dir_fd = open(argv[1], O_RDONLY | O_DIRECTORY);
	if (pip->dir_fd != -1)
	{
		close(pip->dir_fd);
		error_exit("infile is a directory", pip);
	}
	pip->outfile_fd = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pip->outfile_fd == -1)
		error_exit("outfile open error", pip);
}

void	execute_cmd(char *cmd, char **args, char **envp, t_pipex *pip)
{
	char	*path;

	path = cmd_path(cmd, envp);
	if (!path)
		error_exit("command not found :(", pip);
	execve(path, args, envp);
	free(path);
	error_exit("execve error", pip);
}

void	child_one(char **envp, t_pipex	*pip) // for cmd 1
{
	if (dup2(pip->infile_fd, STDIN_FILENO) == -1)
		error_exit("dup2 infile", pip);
	if (dup2(pip->pipefd[1], STDOUT_FILENO) == -1)
		error_exit("dup2 pipe write", pip);
	close_fd(pip);
	execute_cmd(pip->cmd1[0], pip->cmd1, envp, pip);
}

void	child_two(char **envp, t_pipex *pip) // for cmd 2
{
	if (dup2(pip->pipefd[0], STDIN_FILENO) == -1)
		error_exit("dup2 pipe read", pip);
	if (dup2(pip->outfile_fd, STDOUT_FILENO) == -1)
		error_exit("dup2 outfile", pip);
	close_fd(pip);
	execute_cmd(pip->cmd2[0], pip->cmd2, envp, pip);
}

int main(int argc, char *argv[], char **envp)
{
	t_pipex pip; // main pip
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
		error_exit("Syntax Error\ninfile \"cmd1\" \"cmd2\" outfile\n", &pip);
	init_data(&pip, argv);
	if (pipe(pip.pipefd) == -1)
		error_exit("pipe error", &pip);
	pid1 = fork();
	if (pid1 == -1)
		error_exit("fork error", &pip);
	if (pid1 == 0)
		child_one(envp, &pip);
	pid2 = fork();
	if (pid2 == -1)
		error_exit("fork error", &pip);
	if (pid2 == 0)
		child_two(envp, &pip);
	close_fd(&pip);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	free_matrix(pip.cmd1);
	free_matrix(pip.cmd2);
	return (0);
}
