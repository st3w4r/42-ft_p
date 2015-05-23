/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_pi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/19 15:38:13 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/19 15:38:14 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void		ftp_cli_pi_send_cmd(t_cli_ftp *cli_ftp, t_cmd_nvt cmd)
{
	send(cli_ftp->sock, cmd.line_send, ft_strlen(cmd.line_send), 0);
}

t_bool		ftp_cli_pi_search_builtins(t_cli_ftp *cli_ftp, char **args)
{
	t_bool	state;
	int		i;

	i = 0;
	state = FALSE;
	while (g_cmd_cli_list[i].name && state == FALSE)
	{
		if (ft_strcmp(args[0], g_cmd_cli_list[i].name) == 0)
		{
			g_cmd_cli_list[i].builtin(cli_ftp, args);
			state = TRUE;
		}
		++i;
	}
	return (state);
}

int			ftp_cli_pi_create(t_cli_ftp *cli_ftp)
{
	int					r;
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(cli_ftp->port);
	sin.sin_addr.s_addr = inet_addr(cli_ftp->addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		ft_error_str_exit("Connect error\n");
	return (sock);
}
