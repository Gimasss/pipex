/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giomastr <giomastr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:22:20 by giomastr          #+#    #+#             */
/*   Updated: 2025/05/06 18:47:09 by giomastr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"

typedef struct s_pipex
{
	int		pipefd[2];
	int		infile_fd;
	int		outfile_fd;
	int		dir_fd;
	char	**cmd1;
	char	**cmd2;
}	t_pipex;

//	MAIN FILE
void	init_data(t_pipex *pip, char **argv);
void	execute_cmd(char *cmd, char **args, char **envp, t_pipex *pip);
void	child_one(char **envp, t_pipex *pip);// for cmd 1
void	child_two(char **envp, t_pipex *pip); // for cmd 2

//	UTILS FILE
void	error_exit(char *s, t_pipex *pip);
char	*cmd_path(char *cmd, char **envp);
char	*get_full_path(char *path, char *cmd);
void	free_matrix(char **arr);
void	close_fd(t_pipex *pip);

// int		open_file(char *file, int in_or_out);




#endif