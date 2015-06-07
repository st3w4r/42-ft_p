/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_builtins.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/22 14:09:28 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/22 14:09:29 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void	ftp_cli_builtin_cd(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "CWD";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);

}

void	ftp_cli_builtin_pwd(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "PWD";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_ls(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt	cmd;
	t_res		res;
	char		*response;

	ftp_cli_pi_open_data_channel(cli_ftp);
	cmd.name = ft_strdup("LIST");
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	response = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
	res = ftp_parse_response(response);
	ft_putstr(response);
	if (res.code_res == 150)
		ftp_cli_dtp_read_on_channel(cli_ftp);
	free(res.msg_res);
	free(response);
	free(cmd.name);
	free(cmd.line_send);
}

void	ftp_cli_builtin_get(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "RETR";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_put(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "STOR";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_quit(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt	cmd;
	char		*msg;

	cmd.name = "QUIT";
	cmd.args = NULL;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);

	msg = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
	ft_putstr(msg);
	free(msg);
	close(cli_ftp->sock_ctl);
	exit(0);
}

void	ftp_cli_builtin_user(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "USER";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_pass(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "PASS";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

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
