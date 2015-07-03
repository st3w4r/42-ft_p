/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_builtins_transfert.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/03 10:56:56 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/03 10:56:57 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void	ftp_cli_builtin_binary(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "TYPE I";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_ascii(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "TYPE A";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}
