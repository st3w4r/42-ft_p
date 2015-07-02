/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_builtins.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/24 18:23:26 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/24 18:23:28 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

void	ftp_srv_builtin_cd(t_srv_ftp *srv_ftp, char **args)
{
	t_bool allow;

	if (ft_strncmp(args[1], "/", 1) == 0)
	{
		args[1] = ft_strjoin_free_r(srv_ftp->config.path_srv, args[1]);
	}
	if ((ft_arrlen(args) == 2) &&
		(ftp_srv_fs_path_allow(srv_ftp, args[1]) == TRUE) &&
		(chdir(args[1]) == 0))
	{
		ftp_srv_pi_send_response(srv_ftp, 250,
			"Directory successfully changed.");
		return ;
	}
	ftp_srv_pi_send_response(srv_ftp, 550, "Failed to change directory.");
}

void	ftp_srv_builtin_pwd(t_srv_ftp *srv_ftp, char **args)
{
	char	*current_path;
	char	*offset_path;
	char	*msg;

	current_path = ftp_srv_fs_get_path();
	if ((offset_path = ft_strstr(current_path, srv_ftp->config.path_srv)) &&
		(ft_strlen(offset_path) > ft_strlen(srv_ftp->config.path_srv)))
	{
		offset_path = current_path + ft_strlen(srv_ftp->config.path_srv);
	}
	else
		offset_path = "/";
	msg = ft_strjoin_free_l(ft_strjoin("\"", offset_path), "\"");
	ftp_srv_pi_send_response(srv_ftp, 257, msg);
	free(current_path);
	free(msg);
}

void	ftp_srv_builtin_quit(t_srv_ftp *srv_ftp, char **args)
{
	ftp_srv_pi_send_response(srv_ftp, 221, "Goodbye.");
	close(srv_ftp->cs);
	close(srv_ftp->sock);
}

void	ftp_srv_builtin_user(t_srv_ftp *srv_ftp, char **args)
{
	int		i;

	i = 0;
	while (args && args[1] && g_account[i].username)
	{
		if (ft_strcmp(args[1], g_account[i].username) == 0)
		{
			srv_ftp->config.login = args[1];
			ftp_srv_pi_send_response(srv_ftp, 331, "Please pawword.");
			return ;
		}
		++i;
	}
	srv_ftp->config.login = NULL;
	ftp_srv_pi_send_response(srv_ftp, 530, "Login incorrect.");
}

void	ftp_srv_builtin_pass(t_srv_ftp *srv_ftp, char **args)
{
	int		i;

	i = 0;
	if (srv_ftp->config.login == NULL)
	{
		ftp_srv_pi_send_response(srv_ftp, 503, "Login with USER first.");
		return ;
	}
	while (args && args[1] && g_account[i].password)
	{
		if (ft_strcmp(srv_ftp->config.login, g_account[i].username) == 0 &&
			ft_strcmp(args[1], g_account[i].password) == 0)
		{
			srv_ftp->config.is_logged = TRUE;
			ftp_srv_pi_send_response(srv_ftp, 230, "Login successful.");
			return ;
		}
		++i;
	}
	srv_ftp->config.login = NULL;
	ftp_srv_pi_send_response(srv_ftp, 530, "Password incorrect.");
}

void	ftp_srv_builtin_noop(t_srv_ftp *srv_ftp, char **args)
{
	ftp_srv_pi_send_response(srv_ftp, 200, "NOOP ok.");
}

void	ftp_srv_builtin_size(t_srv_ftp *srv_ftp, char **args)
{
	int	fd;
	int	size;

	if (ft_arrlen(args) == 2)
	{
		if ((ftp_srv_fs_file_allow(srv_ftp, &args[1]) == TRUE) &&
			(fd = ftp_srv_fs_open_file(args[1])) != -1)
		{
			size = ftp_srv_fs_size_file(fd);
			close(fd);
			ftp_srv_pi_send_response(srv_ftp, 213, ft_itoa(size));
			return ;
		}
	}
	ftp_srv_pi_send_response(srv_ftp, 550, "Could not get file size.");
}

void	ftp_srv_builtin_mkdir(t_srv_ftp *srv_ftp, char **args)
{
	DIR		*dir;
	char	*file;

	file = ft_strdup(args[1]);
	if (ft_arrlen(args) == 2 &&
		ftp_srv_fs_file_allow(srv_ftp, &file) == TRUE)
	{
		if ((dir = opendir(file)) == NULL)
		{
			if (mkdir(args[1], 0700) == 0)
				ftp_srv_pi_send_response(srv_ftp, 257, "Directory created.");
			else
				ftp_srv_pi_send_response(srv_ftp, 257, "Error create.");
		}
		else
		{
			closedir(dir);
			ftp_srv_pi_send_response(srv_ftp, 550, "Directory exist.");
		}
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 550, "Error");
	free(file);
}

void	ftp_srv_builtin_rmdir(t_srv_ftp *srv_ftp, char **args)
{
	DIR		*dir;
	char	*file;
	char	**new_args;

	file = ft_strdup(args[1]);
	if (ft_arrlen(args) == 2 && ftp_srv_fs_file_allow(srv_ftp, &file) == TRUE)
	{
		if ((dir = opendir(file)) != NULL)
		{
			new_args = (malloc(sizeof(char *) * 4));
			new_args[0] = ft_strdup("/bin/rm");
			new_args[1] = ft_strdup("-rf");
			new_args[2] = ft_strdup(file);
			new_args[3] = NULL;
			ftp_fork_process(new_args);
			ftp_srv_pi_send_response(srv_ftp, 257, "Directory removed.");
			closedir(dir);
			FREE_ARR(new_args);
		}
		else
			ftp_srv_pi_send_response(srv_ftp, 550, "Directory not exist.");
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 550, "Error");
	free(file);
}

void	ftp_srv_builtin_delete(t_srv_ftp *srv_ftp, char **args)
{
	char	*file;

	file = ft_strdup(args[1]);
	if (ft_arrlen(args) == 2 &&
		ftp_srv_fs_file_allow(srv_ftp, &file) == TRUE)
	{
		if (unlink(file) == 0)
			ftp_srv_pi_send_response(srv_ftp, 257, "File removed.");
		else
			ftp_srv_pi_send_response(srv_ftp, 550, "Error remove.");
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 550, "Error");
	free(file);
}
