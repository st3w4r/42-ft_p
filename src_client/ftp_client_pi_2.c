/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_pi_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/04 10:48:29 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/04 10:48:30 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

int		ftp_cli_use_ipv4(t_cli_ftp *cli_ftp, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname("tcp")) == 0)
		return (-1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_len = sizeof(sin);
	sin.sin_family = AF_INET;
	// sin.sin_addr.s_addr = inet_addr(cli_ftp->addr_ctl);
	sin.sin_port = htons(port);
	ft_memcpy((char *)&sin.sin_addr, cli_ftp->host->h_addr_list[0],
										cli_ftp->host->h_length);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		ft_error_str_exit("Connect error\n");
	return (sock);
}

int		ftp_cli_use_ipv6(t_cli_ftp *cli_ftp, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in6 sin;

	if ((proto = getprotobyname("tcp")) == 0)
		return (-1);
	sock = socket(PF_INET6, SOCK_STREAM, proto->p_proto);
	sin.sin6_len = sizeof(sin);
	sin.sin6_family = AF_INET6;
	// sin.sin6_addr = inet_pton(AF_INET6, cli_ftp->addr_ctl, buf_ap);
	ft_memcpy((char *)&sin.sin6_addr, cli_ftp->host->h_addr_list[0],
										cli_ftp->host->h_length);
	sin.sin6_port = htons(port);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		ft_error_str_exit("Connect error\n");
	return (sock);
}
/*

int			ftp_cli_pi_create_ipv6(t_cli_ftp *cli_ftp)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in6	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET6;
	sin.sin_port = htons(cli_ftp->port_ctl);
	sin.sin_addr.s_addr = inet_addr(cli_ftp->addr_ctl);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		ft_error_str_exit("Connect error\n");
	return (sock);
}

int		ftp_srv_dtp_create_channel(t_srv_ftp *srv_ftp)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	(void)srv_ftp;
	proto = getprotobyname("tcp");
	if (proto == 0)
		ft_error_str_exit("Proto error\n");
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = 0;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		ft_error_str_exit("Bind error\n");
	if (listen(sock, 42) == -1)
		ft_error_str_exit("Listen error\n");
	return (sock);
}
*/
