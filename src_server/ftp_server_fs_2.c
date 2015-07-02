/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_fs_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/02 22:46:55 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/02 22:46:58 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

char	*ftp_srv_fs_get_path(void)
{
	char *buf;

	buf = NULL;
	return (getcwd(buf, 0));
}

t_bool	ftp_srv_fs_path_allow(t_srv_ftp *srv_ftp, char *path)
{
	char	*current_path;
	char	*old_path;
	t_bool	allow;

	old_path = ftp_srv_fs_get_path();
	if (chdir(path) == 0)
	{
		current_path = ftp_srv_fs_get_path();
		if (ft_strstr(current_path, srv_ftp->config.path_srv))
			allow = TRUE;
		else
			allow = FALSE;
	}
	else
		allow = FALSE;
	chdir(old_path);
	free(old_path);
	return (allow);
}

t_bool	ftp_srv_fs_file_allow(t_srv_ftp *srv_ftp, char **file)
{
	char		*path_file;
	char		*old_file;
	t_bool		allow;

	if (ft_strncmp(*file, "/", 1) == 0)
	{
		*file = ft_strjoin_free_r(srv_ftp->config.path_srv, *file);
	}
	path_file = ft_strdup(*file);
	old_file = path_file;
	if ((path_file = ft_strrchr(path_file, '/')) != NULL)
	{
		path_file[0] = '\0';
		path_file = old_file;
		allow = ftp_srv_fs_path_allow(srv_ftp, path_file);
	}
	else
		allow = TRUE;
	path_file = old_file;
	free(path_file);
	return (allow);
}
