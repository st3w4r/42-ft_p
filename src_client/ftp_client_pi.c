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
	send(cli_ftp->sock_ctl, cmd.line_send, ft_strlen(cmd.line_send), 0);
}

t_bool		ftp_cli_pi_search_builtins(t_cli_ftp *cli_ftp, char **args)
{
	t_bool	state;
	int		i;

	i = 0;
	state = FALSE;
	while (args && g_cmd_cli_list[i].name && state == FALSE)
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
	int		sock;

	if (cli_ftp->host->h_addrtype == AF_INET6)
		sock = ftp_cli_use_ipv6(cli_ftp, cli_ftp->port_ctl);
	else
		sock = ftp_cli_use_ipv4(cli_ftp, cli_ftp->port_ctl);
	return (sock);
}

char		*ftp_cli_pi_recive_data(int sock)
{
	char	buf[2];
	char	*data;
	int		r;
	t_bool	eol_cr[2];

	eol_cr[0] = FALSE;
	eol_cr[1] = FALSE;
	data = ft_strdup("");
	while (!eol_cr[0])
	{
		if ((r = recv(sock, buf, 1, 0)) < 0)
		{
			ft_error_str("Receive error\n");
			break ;
		}
		buf[r] = '\0';
		if (buf[0] == '\r')
			eol_cr[1] = TRUE;
		else if (eol_cr[1] && buf[0] == '\n')
			eol_cr[0] = TRUE;
		else if (eol_cr[1] && buf[0] != '\n')
			eol_cr[1] = FALSE;
		data = ft_strjoin_free_l(data, buf);
	}
	return (data);
}

void		ftp_cli_pi_open_data_channel(t_cli_ftp *cli_ftp)
{
	t_cmd_nvt	cmd;
	t_res		res;
	char		*data;

	cmd.name = ft_strdup("EPSV");
	cmd.args = NULL;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	data = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
	res = ftp_parse_response(data);
	if (res.code == 227)
	{
		ftp_receive_msg(data);
		ftp_parse_pasv_addr_port(cli_ftp, data);
		cli_ftp->sock_data = ftp_cli_dtp_create_channel(cli_ftp);
	}
	else if (res.code == 229)
	{
		ftp_receive_msg(data);
		ftp_parse_epsv_port(cli_ftp, data);
		cli_ftp->sock_data = ftp_cli_dtp_create_channel(cli_ftp);
	}
	else
		g_need_read = FALSE;
	free(data);
	free(cmd.name);
	free(cmd.line_send);
	free(res.msg);
}
