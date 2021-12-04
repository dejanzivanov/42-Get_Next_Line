/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzivanov <dzivanov@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 18:59:57 by dzivanov          #+#    #+#             */
/*   Updated: 2021/07/08 01:08:04 by dzivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_mem_alloc(int fd, char **line, char **buffer)
{
	if ((read(fd, NULL, 0) < 0) || !line || BUFFER_SIZE <= 0)
		return (-1);
	*buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (-1);
	return (0);
}

char	*ft_line_break(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
		{
			return ((char *)str + i);
		}
		i++;
	}
	return (NULL);
}

char	*ft_temp_check(char **temp, char **line)
{
	char	*ptr;

	ptr = NULL;
	if (*temp)
	{
		ptr = ft_line_break(*temp);
		if (ptr)
		{
			*ptr = '\0';
			*line = ft_strdup(*temp);
			ft_strcpy(*temp, ++ptr);
		}
		else
		{
			*line = ft_strdup(*temp);
			free(*temp);
			*temp = NULL;
		}
	}
	else
	{
		*line = ft_calloc(sizeof(char), 1);
	}
	return (ptr);
}

int	get_next_line(int fd, char **line)
{
	static char	*temp;
	char		*buffer;
	char		*nl_ptr;
	int			flag;

	nl_ptr = NULL;
	flag = 1;
	if (ft_mem_alloc(fd, line, &buffer) < 0)
		return (-1);
	nl_ptr = ft_temp_check(&temp, line);
	while (!nl_ptr && flag > 0)
	{
		flag = read(fd, buffer, BUFFER_SIZE);
		buffer[flag] = '\0';
		nl_ptr = ft_line_break(buffer);
		if (nl_ptr)
		{
			*nl_ptr = '\0';
			temp = ft_strdup(++nl_ptr);
		}
		*line = ft_strjoin(*line, buffer);
	}
	free(buffer);
	return (flag && temp);
}

/*
**	Function get_next_line
**	Initialisation of the function is done by creating
**	static and buffer variables, with additional memory
**	allocation and NULL check.
**
**	Afterwards additional check is performed to see if the
**	function has been called so far(1st time call check).
**
**	If function is called first time we are only going to
**	allocate memory to a new line, and proceed to a while loop.
**
**	Since the while loop is initiated with the flag and null new line
**	pointer, we start reading the number of bytes from the file descriptor
**	using read function, and BUFFER_SIZE.
**
**	While the read function reads number of bytes from the file descriptor
**	we are adding those bytes to a buffer pointer, and add NULL character at
**	end.
**
**	Afterwards we are checking if the new line character is contained between
**	read bytes, if the new line is discovered, we are setting the '\n'
**	character to a '\0'.
**
**	On the next line we are moving cursor one step forwand and pointing to a
**	that position. Which is a new line.
**
**	After that we just join the string to our line variable,
**	and we free buffer so we make sure that there are no memory leaks
**	including (strjoin function).
**
**	Return clause is based on if statement, if certain parameters are positive
**	or negative or 0 (-1, 0, 1). If one of the values is false, we return
**	false (0).
**
**	On the second function call, the function does all the mentioned stuff,
**	but we are starting from the second line, because we have moved the
**	starting position of the pointer to a new line while clearing the static
**	static pointer of starting position.
**
*/