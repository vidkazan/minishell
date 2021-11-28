/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_di.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcody <fcody@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:17:18 by fcody             #+#    #+#             */
/*   Updated: 2021/11/28 08:17:19 by fcody            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

void	double_redirect_input_io(t_elem *elem, int heredoc_pfd[2])
{
	char	*line;
	char	*line_nl;

	line = NULL;
	pipe(heredoc_pfd);
	while (ft_strcmp(elem->cmd[0], line))
	{
		if (line && *line)
			free(line);
		line = readline("> ");
		line_nl = ft_strjoin(line, "\n");
		if (strcmp(elem->cmd[0], line))
			write(heredoc_pfd[1], line_nl, ft_strlen(line_nl));
		free(line_nl);
	}
	if (line && *line)
		free(line);
}

t_elem	*double_redirect_input(t_elem *elem)
{
	int	heredoc_pfd[2];

	double_redirect_input_io(elem, heredoc_pfd);
	close(heredoc_pfd[1]);
	if (elem->data->double_redirect_input_fd)
		close(elem->data->double_redirect_input_fd);
	if (elem->data->simple_redirect_input_fd)
	{
		elem->data->simple_redirect_input_fd = -1;
		close(elem->data->simple_redirect_input_fd);
	}
	elem->data->double_redirect_input_fd = heredoc_pfd[0];
	elem = delete_current_node(elem->data, elem);
	return (elem);
}
