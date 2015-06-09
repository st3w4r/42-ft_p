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

int		ftp_cli_fs_create_file(char *name)
{
	return (open(name, O_CREAT | O_RDWR | O_TRUNC));
}

void	ftp_cli_fs_write_in_file(int fd, char *data)
{
	int	size;

	size = ft_strlen(data);
	if (write(fd, data, size) == -1)
		return ;
}
