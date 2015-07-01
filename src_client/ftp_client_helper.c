/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/25 11:36:10 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/25 11:36:13 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void	ftp_receive_msg(char *msg)
{
	t_res res;

	res = ftp_parse_response(msg);
	if (res.code >= 100 && res.code < 300)
	{
		ft_putstr_fd("\033[1;34m", 1);
		ft_putstr_fd(msg, 1);
		ft_putstr_fd("\033[0m", 1);
	}
	else if (res.code >= 300 && res.code < 600)
	{
		ft_putstr_fd("\033[1;31m", 2);
		ft_putstr_fd(msg, 2);
		ft_putstr_fd("\033[0m", 2);
	}
	else
		ft_putstr_fd(msg, 1);
	free(res.msg);
}

t_res	ftp_parse_response(char *response)
{
	t_res	res;
	char	code[4];

	if (!response)
	{
		res.code = -1;
		res.msg = ft_strdup("Error\r\n");
		return (res);
	}
	res.code = ft_atoi(ft_strncpy(code, response, 3));
	response = ft_strchr(response, ' ');
	res.msg = ft_strdup(response);
	return (res);
}

char	*ftp_create_cmd_line(char *name, char **args)
{
	char *cmd_line;

	cmd_line = ft_strdup(name);
	while (args && *args)
	{
		cmd_line = ft_strjoin_free_l(ft_strjoin_free_l(cmd_line, " "), *args);
		++args;
	}
	cmd_line = ft_strjoin_free_l(cmd_line, "\r\n");
	return (cmd_line);
}

void	ftp_parse_addr_port(t_cli_ftp *cli_ftp, char *msg)
{
	char			*addr;
	char			**msg_arr;
	int				port;

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

void	ftp_fork_process(char **av)
{
	pid_t			pid;

	pid = fork();
	if (pid > 0)
	{
		wait4(pid, 0, 0, 0);
	}
	else if (pid == 0)
	{
		if (execv(av[0], av) == -1)
			ft_error_str("Exec format error.\n");
		exit(0);
	}
}
