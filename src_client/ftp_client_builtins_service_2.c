/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_builtins_service_2.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/03 11:08:33 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/03 11:08:34 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void		ftp_cli_builtin_ls(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt	cmd;
	t_res		res;
	char		*response;

	ftp_cli_pi_open_data_channel(cli_ftp);
	cmd.name = "LIST";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	response = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
	res = ftp_parse_response(response);
	ftp_receive_msg(response);
	if (res.code == 150)
		ftp_cli_dtp_read_on_channel(cli_ftp);
	free(res.msg);
	free(response);
	free(cmd.line_send);
}

static void	ftp_cli_builtin_get_receive(t_cli_ftp *cli_ftp, char **args,
										int argc, t_res res)
{
	int		fd_create;
	int		len;
	char	*data_one;

	if (res.code == 150)
	{
		if ((fd_create = ftp_cli_fs_create_file(args[argc - 1])) != -1)
		{
			while ((data_one = ftp_cli_dtp_read_on_channel_one(cli_ftp, &len)))
			{
				ftp_cli_fs_write_in_file(fd_create, data_one, len);
				free(data_one);
			}
			close(fd_create);
		}
	}
	else
		g_need_read = FALSE;
}

void		ftp_cli_builtin_get(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt	cmd;
	t_res		res;
	char		*response;
	int			argc;

	argc = ft_arrlen(args) - 1;
	if (argc != 1 && argc != 2)
	{
		ft_putstr("usage: get remote-file [local-file]\n");
		g_need_read = FALSE;
		return ;
	}
	ftp_cli_pi_open_data_channel(cli_ftp);
	cmd.name = "RETR";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	response = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
	ftp_receive_msg(response);
	res = ftp_parse_response(response);
	ftp_cli_builtin_get_receive(cli_ftp, args, argc, res);
	free(cmd.line_send);
	free(res.msg);
	free(response);
}

static void	ftp_cli_builtin_put_send(t_cli_ftp *cli_ftp, char **args,
									t_res res)
{
	int		fd;
	int		len_data;
	char	*data;

	if (res.code == 150)
	{
		if ((fd = ftp_cli_fs_open_file(args[1])) != -1)
		{
			while ((data = ftp_cli_fs_read_file(fd, &len_data)))
			{
				ftp_cli_dtp_send_data(cli_ftp, data, len_data);
				free(data);
			}
			ftp_cli_dtp_close_channel(cli_ftp);
			close(fd);
		}
	}
	else
		g_need_read = FALSE;
}

void		ftp_cli_builtin_put(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt	cmd;
	t_res		res;
	char		*response;
	int			argc;

	argc = ft_arrlen(args) - 1;
	if (argc != 1 && argc != 2)
	{
		ft_putstr("usage: put remote-file [local-file]\n");
		g_need_read = FALSE;
		return ;
	}
	ftp_cli_pi_open_data_channel(cli_ftp);
	cmd.name = "STOR";
	cmd.args = argc == 2 ? &(args[2]) : &(args[1]);
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	response = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
	ftp_receive_msg(response);
	res = ftp_parse_response(response);
	ftp_cli_builtin_put_send(cli_ftp, args, res);
	free(cmd.line_send);
	free(res.msg);
	free(response);
}
