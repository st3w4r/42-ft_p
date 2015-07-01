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
	// ftp_srv_dtp_create_channel(srv_ftp);

		// ft_putnbr(srv_ftp->sock_data);
	if (srv_ftp->sock_data != -1)
	{
		ftp_srv_dtp_accept_connection(srv_ftp);
		ftp_srv_pi_send_response(srv_ftp, 150,
			"Here comes the directory listing.");

		new_args = (malloc(sizeof(char *) * 4));
		new_args[0] = ft_strdup("/bin/ls");

		// if (ft_arrlen(args) == 3 || ft_arrlen(args) == 2)
		// {
		if (ft_strchr(args[1], '-') != NULL)
			if (ft_strchr(args[1], 'a') != NULL)
				flags = ft_strdup("-la");
			else
				flags = ft_strdup("-l");
		else
			flags = ft_strdup("-l");
		pos = ft_arrlen(args) -1;
		if (ftp_srv_fs_path_allow(srv_ftp, args[pos]) == TRUE)
			path =  ft_strdup(args[pos]);
		else
			path =  ft_strdup(".");

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



/*
	while ((r = read(srv_ftp->cs, buf, 254)) > 0)
	{
		buf[r] = '\0';

		// ft_putstr(buf);
		ft_putnbr(r);
	}
	ft_putendl("END");
*/


/*
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;

	}*/

	// t_cmd_nvt cmd;
	//
	// cmd.name = "LIST";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_get(t_srv_ftp *srv_ftp, char **args)
{
	int		fd;
	int		len_data;
	char	*data;
	char	*msg;

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
	/*
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;
	}
*/
	// t_cmd_nvt cmd;
	//
	// cmd.name = "RETR";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_put(t_srv_ftp *srv_ftp, char **args)
{
	int		fd_create;
	int		len;
	char	*data_one;

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

/*
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;
	}
*/
	// t_cmd_nvt cmd;
	//
	// cmd.name = "STOR";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_quit(t_srv_ftp *srv_ftp, char **args)
{
	ftp_srv_pi_send_response(srv_ftp, 221, "Goodbye.");
	close(srv_ftp->cs);
	close(srv_ftp->sock);
	/*
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;
	}*/
	// t_cmd_nvt cmd;
	//
	// cmd.name = "QUIT";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_user(t_srv_ftp *srv_ftp, char **args)
{
	// t_bool	state;
	int		i;

	i = 0;
	// state = FALSE;
	while (args && args[1] && g_account[i].username)
	{
		if (ft_strcmp(args[1], g_account[i].username) == 0)
		{
			// g_cmd_cli_list[i].builtin(cli_ftp, args);
			// g_login = args[1];
			srv_ftp->config.login = args[1];
			ftp_srv_pi_send_response(srv_ftp, 331, "Please pawword.");
			// state = TRUE;
			return;
		}
		++i;
	}
	srv_ftp->config.login = NULL;

	// g_login = NULL;
	ftp_srv_pi_send_response(srv_ftp, 530, "Login incorrect.");

	/*
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;
	}
	*/
	// t_cmd_nvt cmd;
	//
	// cmd.name = "USER";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_pass(t_srv_ftp *srv_ftp, char **args)
{
	// ftp_srv_pi_send_response(srv_ftp, 230, "Login successful.");


	int		i;

	i = 0;
	// state = FALSE;
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
			// g_cmd_cli_list[i].builtin(cli_ftp, args);
			// g_login = args[1];
			srv_ftp->config.is_logged = TRUE;
			ftp_srv_pi_send_response(srv_ftp, 230, "Login successful.");
			// g_is_logged = TRUE;

			// ftp_srv_pi_send_response(srv_ftp, 331, "Please pawword.");
			// state = TRUE;
			return;
		}
		++i;
	}
	// g_login = NULL;
// puts(srv_ftp->config.login);

	srv_ftp->config.login = NULL;
	ftp_srv_pi_send_response(srv_ftp, 530, "Password incorrect.");

	/*
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;
	}
	*/
	// t_cmd_nvt cmd;
	//
	// cmd.name = "USER";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_noop(t_srv_ftp *srv_ftp, char **args)
{
	ftp_srv_pi_send_response(srv_ftp, 200, "NOOP ok.");
}

/*
#define	pack2(var, off) \
	(((var[(off) + 0] & 0xff) << 8) | ((var[(off) + 1] & 0xff) << 0))
#define	pack4(var, off) \
	(((var[(off) + 0] & 0xff) << 24) | ((var[(off) + 1] & 0xff) << 16) | \
	((var[(off) + 2] & 0xff) << 8) | ((var[(off) + 3] & 0xff) << 0))
#define	UC(b)	(((int)b)&0xff)
*/

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

	if(ft_arrlen(args) == 2)
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
	// struct stat	info;
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
			closedir(dir);
			FREE_ARR(new_args);
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


/*
void	ftp_srv_builtin_ls(t_srv_ftp *srv_ftp, char **args)
{
	char	*flags;
	char	*path;
	char	**new_args;
	int		pos;
	// ftp_srv_dtp_create_channel(srv_ftp);

		// ft_putnbr(srv_ftp->sock_data);
	if (srv_ftp->sock_data != -1)
	{
		ftp_srv_dtp_accept_connection(srv_ftp);
		ftp_srv_pi_send_response(srv_ftp, 150,
			"Here comes the directory listing.");

		new_args = (malloc(sizeof(char *) * 4));
		new_args[0] = ft_strdup("/bin/ls");

		// if (ft_arrlen(args) == 3 || ft_arrlen(args) == 2)
		// {
		if (ft_strchr(args[1], '-') != NULL)
			if (ft_strchr(args[1], 'a') != NULL)
				flags = ft_strdup("-la");
			else
				flags = ft_strdup("-l");
		else
			flags = ft_strdup("-l");
		pos = ft_arrlen(args) -1;
		if (ftp_srv_fs_path_allow(srv_ftp, args[pos]) == TRUE)
			path =  ft_strdup(args[pos]);
		else
			path =  ft_strdup(".");

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
*/

/*
	struct hostent		*h;
	struct in_addr		**addr_list;
	struct addrinfo		hints;
	struct addrinfo		*srvinfo;
	struct addrinfo		*res;
	struct sockaddr_in	*saddr;
*/
/*
	h = gethostbyname("localhost");
	addr_list = (struct in_addr**)h->h_addr_list;

	printf("%s\n", inet_ntoa(*addr_list[0]));

	// struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP address

	if (getaddrinfo(IP_DATA_CHANNEL, NULL, &hints, &srvinfo) != 0)
	{
		ft_error_str("Erreur getaddrinfo\n");
		ftp_srv_pi_send_response(srv_ftp, 425, "Cannot open data connection.");
		return ;
	}

	res = srvinfo;
	saddr = (struct sockaddr_in*)res->ai_addr;
	addr[0] = ft_itoa(saddr->sin_addr.s_addr >> 0 & 0xff);
	addr[1] = ft_itoa(saddr->sin_addr.s_addr >> 8 & 0xff);
	addr[2] = ft_itoa(saddr->sin_addr.s_addr >> 16 & 0xff);
	addr[3] = ft_itoa(saddr->sin_addr.s_addr >> 24 & 0xff);
	port[0] = ft_itoa(sin.sin_port >> 0 & 0xff);
	port[1] = ft_itoa(sin.sin_port >> 8 & 0xff);
*/
	/*
		msg = ft_strjoin(a0, ",");
		msg = ft_strjoin(msg, a1);
		msg = ft_strjoin(msg, ",");
		msg = ft_strjoin(msg, a2);
		msg = ft_strjoin(msg, ",");
		msg = ft_strjoin(msg, a3);
*/
		// puts(ft_itoa(saddr->sin_addr.s_addr >> 0 & 0xff));
		// puts(ft_itoa(saddr->sin_addr.s_addr >> 8 & 0xff));
		// puts(ft_itoa(saddr->sin_addr.s_addr >> 16 & 0xff));
		// puts(ft_itoa(saddr->sin_addr.s_addr >> 24 & 0xff));
		// msg = ft_strjoin(msg, a3);
		// msg = ft_strjoin(msg, ",");
		// msg = ft_itoa(saddr->sin_addr.s_addr & 0xff << 16);
		// msg = ft_strjoin(msg, ",");
		// msg = ft_itoa(saddr->sin_addr.s_addr & 0xff << 24);
		// msg = ft_strjoin(msg, " ");

	// struct addrinfo *res;
	// for(res = servinfo; res != NULL; res = res->ai_next)
	// {
    /* ideally look at the sa_family here to make sure it is AF_INET before casting */
    // struct sockaddr_in* saddr = (struct sockaddr_in*)res->ai_addr;
    // printf("hostname: %s\n", inet_ntoa(saddr->sin_addr));
	// }
	// char *msg_arr[8] = {"Entering Passive Mode (",
	// 					addr[0], ",",
	// 					addr[1], ",",
	// 					addr[2], ",",
	// 					addr[3], ",",
	// 					port[0], ",",
	// 					port[1], ")",
	// 					NULL};
	//
	// msg = ft_arrjoin(msg_arr);
	// msg = ft_strdup("Entering Passive Mode (");
	// addr_str = ft_arrjoin_separator(addr, ",");
	// port_str = ft_arrjoin_separator(port, ",");

	// addr_str = ft_arrjoin(addr);
	// addr_str = ft_arrjoin(port);
	// msg = ft_strjoin("Entering Passive Mode (", addr_str);
	// msg = ft_strjoin_free_l(msg, port_str);
	// msg = ft_strjoin_free_l(msg, ")");

/*
	getifaddrs(&ifa);
	sa = (struct sockaddr_in *) ifa->ifa_addr;
	addr = inet_ntoa(sa->sin_addr);
	printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, addr);

	// while (ifa)
	// {
	sa = (struct sockaddr_in *) ifa->ifa_addr;
	addr = inet_ntoa(sa->sin_addr);
	printf("%s\n", addr);

	sa = (struct sockaddr_in *) ifa->ifa_next->ifa_addr;
	addr = inet_ntoa(sa->sin_addr);
	printf("%s\n", addr);
	// }
*/
/*
	struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;

    getifaddrs(&ifap);
    // for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
	ifa = ifap;
	while (ifa)
	{
        // if (ifa->ifa_addr->sa_family==AF_INET)
		// {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, addr);
        // }
		ifa = ifa->ifa_next;
    }
    freeifaddrs(ifap);
*/
/*
	printf("%d\n", data_addr.si_su.su_sin.sin_port);

	memset(&data_addr, 0, sizeof(data_addr));
	data_addr.su_family = AF_INET;
	data_addr.su_len = sizeof(struct sockaddr_in);
	data_addr.si_su.su_sin.sin_addr.s_addr =
		htonl(pack4(addr, 0));
	data_addr.su_port = htons(pack2(port, 0));
*/
	// char *name;
	// int nm;
	// nm = gethostname(name, len);
// printf("Name: %s | %d\n", name, nm);
	// inet_ntoa(sin.sin_addr);
	// msg = ft_itoa(ntohs(sin.sin_port));
// puts(inet_ntoa(sin.sin_addr));
// printf("%d\n", ntohs(sin.sin_addr.s_addr));

	// msg = ft_strjoin("Entering Passive Mode (", inet_ntoa(sin.sin_addr));
