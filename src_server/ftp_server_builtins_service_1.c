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

void	ftp_srv_builtin_pwd(t_srv_ftp *srv_ftp, char **args)
{
	char	*current_path;
	char	*offset_path;
	char	*msg;

	(void)args;
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
