/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_builtins_transfert.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/02 22:27:25 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/02 22:27:26 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

static char	*ftp_srv_builtin_pasv_init(struct ifaddrs *ifa,
	struct sockaddr_in sin)
{
	struct in_addr		i_addr;
	struct sockaddr_in	*sa;
	char				*msg;
	char				*addr;
	char				*port[2];

	while (ifa)
	{
		if (ifa->ifa_addr->sa_family == AF_INET &&
			!ft_strcmp(ifa->ifa_name, "en0"))
		{
			sa = (struct sockaddr_in *)ifa->ifa_addr;
			i_addr = sa->sin_addr;
		}
		ifa = ifa->ifa_next;
	}
	addr = ft_strdup(inet_ntoa(i_addr));
	addr = ft_strreplace_char(addr, '.', ',');
	port[0] = ft_itoa(sin.sin_port >> 0 & 0xff);
	port[1] = ft_itoa(sin.sin_port >> 8 & 0xff);
	msg = ft_str_arrjoin((char *[]){"Entering Passive Mode (", addr,
									",", port[0], ",", port[1], ").", NULL});
	ft_str_arrfree((char *[]){addr, port[0], port[1], NULL});
	return (msg);
}

void		ftp_srv_builtin_pasv(t_srv_ftp *srv_ftp, char **args)
{
	struct sockaddr_in	sin;
	socklen_t			len;
	char				*msg;
	struct ifaddrs		*ifap;

	(void)args;
	srv_ftp->sock_data = ftp_srv_dtp_create_channel(srv_ftp);
	len = sizeof(sin);
	if ((getsockname(srv_ftp->sock_data, (struct sockaddr *)&sin, &len) != 0) ||
		(getifaddrs(&ifap) != 0))
	{
		ftp_srv_pi_send_response(srv_ftp, 425, "Cannot open data connection.");
		return ;
	}
	msg = ftp_srv_builtin_pasv_init(ifap, sin);
	freeifaddrs(ifap);
	ftp_srv_pi_send_response(srv_ftp, 227, msg);
	free(msg);
}

void		ftp_srv_builtin_epsv(t_srv_ftp *srv_ftp, char **args)
{
	struct sockaddr_in6	sin;
	socklen_t			len;
	char				*msg;
	struct ifaddrs		*ifap;

	(void)args;
	srv_ftp->sock_data = ftp_srv_dtp_create_channel(srv_ftp);
	len = sizeof(sin);
	if ((getsockname(srv_ftp->sock_data, (struct sockaddr *)&sin, &len) != 0) ||
		(getifaddrs(&ifap) != 0))
	{
		ftp_srv_pi_send_response(srv_ftp, 425, "Cannot open data connection.");
		return ;
	}
	msg = ft_str_arrjoin((char *[]){"Entering Extended Passive Mode (|||",
								ft_itoa(ntohs(sin.sin6_port)), "|).", NULL});
	freeifaddrs(ifap);
	ftp_srv_pi_send_response(srv_ftp, 229, msg);
	free(msg);
}

void		ftp_srv_builtin_port(t_srv_ftp *srv_ftp, char **args)
{
	(void)args;
	ftp_srv_pi_send_response(srv_ftp, 500, "Illegal PORT command.");
}

void		ftp_srv_builtin_type(t_srv_ftp *srv_ftp, char **args)
{
	if (ft_arrlen(args) == 2)
	{
		if (!ft_strcmp(ft_str_toupper(args[1]), "I"))
		{
			srv_ftp->config.type = BINARY;
			ftp_srv_pi_send_response(srv_ftp, 200, "Switching to Binary mode.");
		}
		else if (!ft_strcmp(ft_str_toupper(args[1]), "A"))
		{
			srv_ftp->config.type = ASCII;
			ftp_srv_pi_send_response(srv_ftp, 200, "Switching to ASCII mode.");
		}
		else
			ftp_srv_pi_send_response(srv_ftp, 504,
				"Command not implemented for that parameter.");
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 500, "Unrecognised TYPE command.");
}
