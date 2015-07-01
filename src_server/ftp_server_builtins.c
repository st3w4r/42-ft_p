/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_builtins.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/24 18:23:26 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/24 18:23:28 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

void	ftp_srv_builtin_cd(t_srv_ftp *srv_ftp, char **args)
{
	t_bool allow;

	if (ft_strncmp(args[1], "/", 1) == 0)
	{
		args[1] = ft_strjoin_free_r(srv_ftp->config.path_srv, args[1]);
	}
	if ((ft_arrlen(args) == 2) &&
		(ftp_srv_fs_path_allow(srv_ftp, args[1]) == TRUE) &&
		(chdir(args[1]) == 0))
	{
		ftp_srv_pi_send_response(srv_ftp, 250,
			"Directory successfully changed.");
		return ;
	}
	ftp_srv_pi_send_response(srv_ftp, 550, "Failed to change directory.");
}

void	ftp_srv_builtin_pwd(t_srv_ftp *srv_ftp, char **args)
{
	char	*current_path;
	char	*offset_path;
	char	*msg;

	current_path = ftp_srv_fs_get_path();
	if ((offset_path = ft_strstr(current_path, srv_ftp->config.path_srv)) &&
		(ft_strlen(offset_path) > ft_strlen(srv_ftp->config.path_srv)))
	{
		offset_path = current_path + ft_strlen(srv_ftp->config.path_srv);
	}
	else
		offset_path = "/";
	msg = ft_strjoin_free_l(ft_strjoin("\"", offset_path), "\"");
	ftp_srv_pi_send_response(srv_ftp, 257, msg);
	free(current_path);
	free(msg);
}

void	ftp_srv_builtin_ls(t_srv_ftp *srv_ftp, char **args)
{
	char	*flags;
	char	*path;
	char	**new_args;
	int		pos;

	if (srv_ftp->sock_data != -1)
	{
		ftp_srv_dtp_accept_connection(srv_ftp);
		ftp_srv_pi_send_response(srv_ftp, 150,
			"Here comes the directory listing.");
		new_args = (malloc(sizeof(char *) * 4));
		new_args[0] = ft_strdup("/bin/ls");
		if (ft_strchr(args[1], '-') != NULL)
			if (ft_strchr(args[1], 'a') != NULL)
				flags = ft_strdup("-la");
			else
				flags = ft_strdup("-l");
		else
			flags = ft_strdup("-l");
		pos = ft_arrlen(args) - 1;
		if (ftp_srv_fs_path_allow(srv_ftp, args[pos]) == TRUE)
			path = ft_strdup(args[pos]);
		else
			path = ft_strdup(".");
		new_args[1] = flags;
		new_args[2] = path;
		new_args[3] = NULL;
		ftp_redirect_fd(srv_ftp->cs_data, STDOUT_FILENO);
		ftp_redirect_fd(srv_ftp->cs_data, STDERR_FILENO);
		ftp_fork_process(new_args);
		ftp_redirect_fd(STDIN_FILENO, STDOUT_FILENO);
		ftp_redirect_fd(STDIN_FILENO, STDERR_FILENO);
		ftp_srv_dtp_close_channel(srv_ftp);
		ftp_srv_pi_send_response(srv_ftp, 226, "Directory send OK");
		FREE_ARR(new_args);
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 425, "Use PORT or PASV first.");
}

void	ftp_srv_builtin_get(t_srv_ftp *srv_ftp, char **args)
{
	int		fd;
	int		len_data;
	char	*data;
	char	*msg;
	char	*new_data;

	if (srv_ftp->sock_data != -1)
	{
		ftp_srv_dtp_accept_connection(srv_ftp);
		if ((ftp_srv_fs_file_allow(srv_ftp, &args[1]) == TRUE) &&
			(fd = ftp_srv_fs_open_file(args[1])) != -1)
		{
			msg = ft_strdup("Opening BINARY mode data connection for ");
			msg = ft_strjoin_free_l(msg, args[1]);
			msg = ft_strjoin_free_l(msg, " (");
			msg = ft_strjoin_free_lr(msg, ft_itoa(ftp_srv_fs_size_file(fd)));
			msg = ft_strjoin_free_l(msg, " bytes).");
			ftp_srv_pi_send_response(srv_ftp, 150, msg);
			free(msg);
			while ((data = ftp_srv_fs_read_file(fd, &len_data)))
			{
				if (srv_ftp->config.type == ASCII)
				{
					new_data = ftp_srv_crlf(data, SRV_CONF, CLI_CONF);
					ftp_srv_dtp_send_data(srv_ftp, new_data, len_data);
					free(new_data);
				}
				else
					ftp_srv_dtp_send_data(srv_ftp, data, len_data);
				free(data);
			}
			ftp_srv_dtp_close_channel(srv_ftp);
			ftp_srv_pi_send_response(srv_ftp, 226, "SUCCESS");
			close(fd);
		}
		else
			ftp_srv_pi_send_response(srv_ftp, 550, "Failed to open file.");
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 425, "Use PORT or PASV first.");
}

void	ftp_srv_builtin_put(t_srv_ftp *srv_ftp, char **args)
{
	int		fd_create;
	int		len;
	char	*data_one;
	char	*new_data;

	if (srv_ftp->sock_data != -1)
	{
		ftp_srv_dtp_accept_connection(srv_ftp);
		ftp_srv_pi_send_response(srv_ftp, 150,
			"Opening BINARY mode data connection for");
		if (((fd_create = ftp_srv_fs_create_file(args[1])) != -1) &&
			(ftp_srv_fs_file_allow(srv_ftp, &args[1]) == TRUE))
		{
			while ((data_one = ftp_srv_dtp_read_on_channel_one(srv_ftp, &len)))
			{
				if (srv_ftp->config.type == ASCII)
				{
					new_data = ftp_srv_crlf(data_one, CLI_CONF, SRV_CONF);
					ftp_srv_fs_write_in_file(fd_create, new_data, len);
					free(new_data);
				}
				else
					ftp_srv_fs_write_in_file(fd_create, data_one, len);
				free(data_one);
			}
			ftp_srv_dtp_close_channel(srv_ftp);
			ftp_srv_pi_send_response(srv_ftp, 226, "SUCCESS");
			close(fd_create);
		}
		else
			ftp_srv_pi_send_response(srv_ftp, 550, "Failed to create file.");
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 425, "Use PORT or PASV first.");
}

void	ftp_srv_builtin_quit(t_srv_ftp *srv_ftp, char **args)
{
	ftp_srv_pi_send_response(srv_ftp, 221, "Goodbye.");
	close(srv_ftp->cs);
	close(srv_ftp->sock);
}

void	ftp_srv_builtin_user(t_srv_ftp *srv_ftp, char **args)
{
	int		i;

	i = 0;
	while (args && args[1] && g_account[i].username)
	{
		if (ft_strcmp(args[1], g_account[i].username) == 0)
		{
			srv_ftp->config.login = args[1];
			ftp_srv_pi_send_response(srv_ftp, 331, "Please pawword.");
			return ;
		}
		++i;
	}
	srv_ftp->config.login = NULL;
	ftp_srv_pi_send_response(srv_ftp, 530, "Login incorrect.");
}

void	ftp_srv_builtin_pass(t_srv_ftp *srv_ftp, char **args)
{
	int		i;

	i = 0;
	if (srv_ftp->config.login == NULL)
	{
		ftp_srv_pi_send_response(srv_ftp, 503, "Login with USER first.");
		return ;
	}
	while (args && args[1] && g_account[i].password)
	{
		if (ft_strcmp(srv_ftp->config.login, g_account[i].username) == 0 &&
			ft_strcmp(args[1], g_account[i].password) == 0)
		{
			srv_ftp->config.is_logged = TRUE;
			ftp_srv_pi_send_response(srv_ftp, 230, "Login successful.");
			return ;
		}
		++i;
	}
	srv_ftp->config.login = NULL;
	ftp_srv_pi_send_response(srv_ftp, 530, "Password incorrect.");
}

void	ftp_srv_builtin_noop(t_srv_ftp *srv_ftp, char **args)
{
	ftp_srv_pi_send_response(srv_ftp, 200, "NOOP ok.");
}

void	ftp_srv_builtin_pasv(t_srv_ftp *srv_ftp, char **args)
{
	int					port_ret;
	struct sockaddr_in	sin;
	socklen_t			len;
	char				*msg;
	char				*addr[4];
	char				*port[2];
	struct ifaddrs		*ifa;
	struct ifaddrs		*ifap;
	struct sockaddr_in	*sa;
	int					s_addr;

	srv_ftp->sock_data = ftp_srv_dtp_create_channel(srv_ftp);
	len = sizeof(sin);
	if (getsockname(srv_ftp->sock_data, (struct sockaddr *)&sin, &len) != 0)
	{
		ftp_srv_pi_send_response(srv_ftp, 425, "Cannot open data connection.");
		return ;
	}
	if (getifaddrs(&ifap) != 0)
	{
		ftp_srv_pi_send_response(srv_ftp, 425, "Cannot open data connection.");
		return ;
	}
	ifa = ifap;
	while (ifa)
	{
		if (ifa->ifa_addr->sa_family == AF_INET &&
			!ft_strcmp(ifa->ifa_name, "en0"))
		{
			sa = (struct sockaddr_in *)ifa->ifa_addr;
			s_addr = sa->sin_addr.s_addr;
		}
		ifa = ifa->ifa_next;
	}
	freeifaddrs(ifap);
	addr[0] = ft_itoa(s_addr >> 0 & 0xff);
	addr[1] = ft_itoa(s_addr >> 8 & 0xff);
	addr[2] = ft_itoa(s_addr >> 16 & 0xff);
	addr[3] = ft_itoa(s_addr >> 24 & 0xff);
	port[0] = ft_itoa(sin.sin_port >> 0 & 0xff);
	port[1] = ft_itoa(sin.sin_port >> 8 & 0xff);
	msg = ft_str_arrjoin((char *[]){"Entering Passive Mode (", addr[0], ",",
		addr[1], ",", addr[2], ",", addr[3], ",", port[0], ",", port[1], ")",
		NULL});
	ftp_srv_pi_send_response(srv_ftp, 227, msg);
	ft_str_arrfree((char *[]){msg, addr[0], addr[1], addr[2], addr[3], port[0],
					port[1], NULL});
}

void	ftp_srv_builtin_port(t_srv_ftp *srv_ftp, char **args)
{
	ftp_srv_pi_send_response(srv_ftp, 500, "Illegal PORT command.");
}

void	ftp_srv_builtin_type(t_srv_ftp *srv_ftp, char **args)
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

void	ftp_srv_builtin_size(t_srv_ftp *srv_ftp, char **args)
{
	int	fd;
	int	size;

	if (ft_arrlen(args) == 2)
	{
		if ((ftp_srv_fs_file_allow(srv_ftp, &args[1]) == TRUE) &&
			(fd = ftp_srv_fs_open_file(args[1])) != -1)
		{
			size = ftp_srv_fs_size_file(fd);
			close(fd);
			ftp_srv_pi_send_response(srv_ftp, 213, ft_itoa(size));
			return ;
		}
	}
	ftp_srv_pi_send_response(srv_ftp, 550, "Could not get file size.");
}

void	ftp_srv_builtin_mkdir(t_srv_ftp *srv_ftp, char **args)
{
	DIR		*dir;
	char	*file;

	file = ft_strdup(args[1]);
	if (ft_arrlen(args) == 2 &&
		ftp_srv_fs_file_allow(srv_ftp, &file) == TRUE)
	{
		if ((dir = opendir(file)) == NULL)
		{
			if (mkdir(args[1], 0700) == 0)
				ftp_srv_pi_send_response(srv_ftp, 257, "Directory created.");
			else
				ftp_srv_pi_send_response(srv_ftp, 257, "Error create.");
		}
		else
		{
			closedir(dir);
			ftp_srv_pi_send_response(srv_ftp, 550, "Directory exist.");
		}
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 550, "Error");
	free(file);
}

void	ftp_srv_builtin_rmdir(t_srv_ftp *srv_ftp, char **args)
{
	DIR		*dir;
	char	*file;
	char	**new_args;

	file = ft_strdup(args[1]);
	if (ft_arrlen(args) == 2 &&
		ftp_srv_fs_file_allow(srv_ftp, &file) == TRUE)
	{
		if ((dir = opendir(file)) != NULL)
		{
			new_args = (malloc(sizeof(char *) * 4));
			new_args[0] = ft_strdup("/bin/rm");
			new_args[1] = ft_strdup("-rf");
			new_args[2] = ft_strdup(file);
			new_args[3] = NULL;
			ftp_fork_process(new_args);
			ftp_srv_pi_send_response(srv_ftp, 257, "Directory removed.");
			closedir(dir), FREE_ARR(new_args);
		}
		else
			ftp_srv_pi_send_response(srv_ftp, 550, "Directory not exist.");
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 550, "Error");
	free(file);
}

void	ftp_srv_builtin_delete(t_srv_ftp *srv_ftp, char **args)
{
	char	*file;

	file = ft_strdup(args[1]);
	if (ft_arrlen(args) == 2 &&
		ftp_srv_fs_file_allow(srv_ftp, &file) == TRUE)
	{
		if (unlink(file) == 0)
			ftp_srv_pi_send_response(srv_ftp, 257, "File removed.");
		else
			ftp_srv_pi_send_response(srv_ftp, 550, "Error remove.");
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 550, "Error");
	free(file);
}
