/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_helper_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/04 19:17:57 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/04 19:17:58 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void	ftp_parse_pasv_addr_port(t_cli_ftp *cli_ftp, char *msg)
{
	char	*addr;
	char	**msg_arr;
	int		port;

	msg = ft_strchr(msg, '(');
	msg = ft_strreplace_char(++msg, ')', '\0');
	msg_arr = ft_strsplit(msg, ',');
	addr = ft_strdup(msg_arr[0]);
	addr = ft_strjoin_free_l(addr, ".");
	addr = ft_strjoin_free_l(addr, msg_arr[1]);
	addr = ft_strjoin_free_l(addr, ".");
	addr = ft_strjoin_free_l(addr, msg_arr[2]);
	addr = ft_strjoin_free_l(addr, ".");
	addr = ft_strjoin_free_l(addr, msg_arr[3]);
	port = 256 * ft_atoi(msg_arr[4]) + ft_atoi(msg_arr[5]);
	cli_ftp->addr_data = addr;
	cli_ftp->port_data = port;
	FREE_ARR(msg_arr);
}

void	ftp_parse_epsv_port(t_cli_ftp *cli_ftp, char *msg)
{
	int		port;

	msg = ft_strchr(msg, '(');
	msg = ft_strchr(++msg, '|');
	msg = ft_strchr(++msg, '|');
	msg = ft_strchr(++msg, '|');
	msg = ft_strreplace_char(++msg, '|', '\0');
	port = ft_atoi(msg);
	cli_ftp->port_data = port;
}
