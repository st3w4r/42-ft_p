/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_conversion.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/01 12:25:32 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/01 12:25:33 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

char	*ftp_srv_unix_to_dos(char *buf)
{
	size_t	i;
	size_t	j;
	size_t	nb;
	char	*new_str;

	nb = ft_str_count_char(buf, '\n');
	new_str = ft_strnew(ft_strlen(buf) + nb + 1);
	i = 0;
	j = 0;
	while (buf && buf[i] && i < ft_strlen(buf))
	{
		if (buf[i] == '\n')
		{
			new_str[j++] = '\r';
			new_str[j] = '\n';
		}
		else
			new_str[j] = buf[i];
		i++;
		j++;
	}
	new_str[j] = '\0';
	return (new_str);
}

char	*ftp_srv_dos_to_unix(char *buf)
{
	size_t	i;
	size_t	j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = ft_strnew(ft_strlen(buf) + 1);
	while (buf[i] && i < (ft_strlen(buf)))
	{
		if (buf[i] == '\r' && buf[i + 1] == '\n')
		{
			new_str[j] = '\n';
			i += 2;
			j++;
		}
		else
		{
			new_str[j] = buf[i];
			i++;
			j++;
		}
	}
	new_str[j] = '\0';
	return (new_str);
}

char	*ftp_srv_crlf(char *data, int from, int to)
{
	if (from == UNIX && to == DOS)
		return (ftp_srv_unix_to_dos(data));
	else if (from == DOS && to == UNIX)
		return (ftp_srv_dos_to_unix(data));
	return (data);
}
