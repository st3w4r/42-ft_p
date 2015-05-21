/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/16 13:29:05 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/16 13:29:12 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

static void		ftp_redirect_fd(int srcfd, int dstfd)
{
	if (dup2(srcfd, dstfd) == -1)
		ft_error_str("Error redirect fd");
}

static void		ftp_fork_process(char *path, char **av)
{
	pid_t			pid;
	int				options;
	int				*status;
	struct rusage	*rusage;

	pid = fork();
	if (pid > 0)
	{
		wait4(pid, status, options, rusage);
	}
	else if (pid == 0)
	{
		if (execv(av[0], av) == -1)
			ft_error_str("Exec format error.\n");
		exit(0);
	}
}

static void		ftp_read_on_socket(int cs)
{
	char	*path;
	char	**args;
	char	buf[1024];
	int		r;

	while ((r = read(cs, buf, 1023)) > 0)
	{
		buf[r] = '\0';
		args = ft_strsplit(buf, ' ');
		ftp_fork_process(args[0], args);
		ft_arrfree(&args);
		send(cs, NULL, r, 0);
	}
}

static int		ftp_create_server(int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);

	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		ft_error_str_exit("Bind error\n");
	if (listen(sock, 42) == -1)
		ft_error_str_exit("Listen error\n");

	return(sock);
}

void		ftp_create_socket(int port)
{
	pid_t				pid;
	int					sock;
	int					cs;
	unsigned int		cslen;
	struct sockaddr_in	csin;

	sock = ftp_create_server(port);
	while (42)
	{
		cs = accept(sock, (struct sockaddr*)&csin,  &cslen);
		// ftp_redirect_fd(cs, STDOUT_FILENO);
		// ftp_redirect_fd(cs, STDERR_FILENO);
		pid = fork();
		if (pid > 0)
		{
			close(cs);
		}
		else if (pid == 0)
		{
			ftp_read_on_socket(cs);
			close(sock);
			exit(0);
		}
		// send(cs, "ok1234567890", 7, 0);
	}
}
