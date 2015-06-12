/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_fs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/19 15:40:05 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/19 15:40:06 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

int		ftp_cli_fs_open_file(char *name)
{
	return (open(name, O_RDONLY));
}

int		ftp_cli_fs_create_file(char *name)
{
	return (open(name, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR
		 		| S_IRGRP | S_IROTH));
}

t_bool	ftp_cli_fs_write_in_file(int fd, char *data, int len)
{
	if (write(fd, data, len) != -1)
		return TRUE;
	return FALSE;
}

char	*ftp_cli_fs_read_file(int fd, int *len)
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

char	*ftp_cli_fs_get_path(void)
{
	char *buf;

	buf = NULL;
	return (getcwd(buf, 0));
}
