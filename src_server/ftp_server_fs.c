/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_fs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/19 15:40:35 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/19 15:40:37 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

int		ftp_srv_fs_create_file(char *name)
{
	return (open(name, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR
		 		| S_IRGRP | S_IROTH));
}

int		ftp_srv_fs_open_file(char *name)
{
	return (open(name, O_RDONLY));
}

char	*ftp_srv_fs_read_file(int fd, int *len)
{
	int		r;
	char	*buf;

	if (!(buf = (char*)malloc(sizeof(char) * 1024)))
		ft_malloc_error();
	if ((r = read(fd, buf, 1024)) > 0)
	{
		*len = r;
		return (buf);
	}
	else
		return (NULL);
}

t_bool	ftp_srv_fs_write_in_file(int fd, char *data, int len)
{
	if (write(fd, data, len) != -1)
		return TRUE;
	return FALSE;
}


int		ftp_srv_fs_size_file(int fd)
{
	struct stat info;

	if (fstat(fd, &info) < 0)
		return (-1);
	return (info.st_size);
}

char	*ftp_srv_fs_get_path(void)
{
	char *buf;

	buf = NULL;
	return (getcwd(buf, 0));
}

t_bool	ftp_srv_fs_path_allow(t_srv_ftp *srv_ftp, char *path)
{
	char *current_path;
	char *old_path;

	old_path = ftp_srv_fs_get_path();
	if (chdir(path) == 0)
	{
		current_path = ftp_srv_fs_get_path();
		if ((ft_strlen(current_path) >= ft_strlen(srv_ftp->config.path_srv)))
		{
			chdir(old_path);
			return (TRUE);
		}
	}
	chdir(old_path);
	return (FALSE);
}
