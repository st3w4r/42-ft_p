/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_pi_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/02 22:50:57 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/02 22:50:58 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

void	ftp_srv_pi_send_response(t_srv_ftp *srv_ftp, int co, char *msg)
{
	char *res;

	res = ft_itoa(co);
	res = ft_strjoin_free_l(res, " ");
	res = ft_strjoin_free_l(res, msg);
	res = ft_strjoin_free_l(res, "\r\n");
	send(srv_ftp->cs, res, ft_strlen(res), 0);
}

int		ftp_srv_use_ipv4(int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname("tcp")) == 0)
		return (-1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_len = sizeof(sin);
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) < 0)
		ft_error_str_exit("Connect error\n");
	return (sock);
}

int		ftp_srv_use_ipv6(int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in6 sin;

	if ((proto = getprotobyname("tcp")) == 0)
		return (-1);
	sock = socket(PF_INET6, SOCK_STREAM, proto->p_proto);
	sin.sin6_len = sizeof(sin);
	sin.sin6_family = AF_INET6;
	sin.sin6_addr = in6addr_any;
	sin.sin6_port = htons(port);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) < 0)
		ft_error_str_exit("Connect error\n");
	return (sock);
}
