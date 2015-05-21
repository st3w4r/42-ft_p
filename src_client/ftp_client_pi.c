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

void	ftp_cli_pi_send_cmd(t_cmd cmd)
{

}

int		ftp_cli_pi_search_builtins(t_cli_ftp *cli_ftp, char **agrs)
{
	int		state;

	state = 0;
	if (ft_strcmp(args[0], "cd") == 0)
		ftp_cli_builtin_cd(cli_ftp, args), state = 1;
		// ftp_cli_pi_cmd(cli_ftp, "CWD", args);
	else if (ft_strcmp(args[0], "pwd") == 0)
		ftp_cli_builtin_pwd(cli_ftp, args), state = 1;
	else if (ft_strcmp(args[0], "ls") == 0)
		ftp_cli_builtin_ls(cli_ftp, args), state = 1;
	else if (ft_strcmp(args[0], "get") == 0)
		ftp_cli_builtin_get(cli_ftp, args), state = 1;
	else if (ft_strcmp(args[0], "put") == 0)
		ftp_cli_builtin_put(cli_ftp, args), state = 1;
	else if (ft_strcmp(args[0], "quit") == 0)
		ftp_cli_builtin_quit(cli_ftp, args), state = 1;
	return (state);
}

int		ftp_cli_pi_create(t_cli_ftp *cli_ftp)
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
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		ft_error_str_exit("Connect error\n");
	return(sock);
}
