/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_options.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eherrero <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 18:44:24 by eherrero          #+#    #+#             */
/*   Updated: 2019/12/16 17:26:35 by eherrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

//Unificar parte comun
//Tratar precision y width
void	ft_print_data(t_data *data)
{
	printf("char: >%c<\n", data->str[data->pos]);
	printf("actual_type->%c\n", data->actual_type);
	printf("pos->%d\n", data->pos);
	printf("width->%d\n", data->width);
	printf("precision->%d\n", data->precision);
	printf("minus_flag->%d\n", data->minus_flag);
	printf("zero_flag->%d\n", data->zero_flag);
	printf("str->%s\n", data->str);
	printf("out->%s\n", data->out);
	printf("set_flags->%s\n", data->set_flags);
	printf("set_types->%s\n", data->set_types);
}

static char *ft_width_treatment(t_data *data, int len, char *str)
{
	char	*ret;
	char	*memsetdest;
	char	*memmovedest;
	char	blank;

	if (data->width < 0)
	{
		data->width *= -1;
		data->minus_flag = 1;
	}
	if (len >= data->width)
		return (ft_strdup(str));
	if (!(ret = ft_calloc(data->width + 1, sizeof(char))))
		return (0);
	blank = data->minus_flag || !data->zero_flag ? ' ' : '0';
	if (!data->minus_flag)
	{
		memsetdest = ret;
		memmovedest = ret + data->width - len;
	}
	else
	{
		memsetdest = ret + len;
		memmovedest = ret;
	}
	ft_memset(memsetdest, blank, data->width - len);
	ft_memmove(memmovedest, str, len);
	return (ret);
}

static int	ft_size_p(char type, int size, int precision)
{
	if (type == 'c')
		return (size);
	if (type == 's')
		return (size > precision ? precision : size);
	if (ft_inset("di", type))
		return (size > precision ? size : precision);
	return (-1);
}

static char	*ft_precision_treatment(t_data *data, char *str, int *size)
{
	char	*ret;
	int		len;
	int		precision;

	len = *size;
	precision = data->precision < 0 ? len : data->precision;
	//printf("entra precision = %d tipo: %c\n", data->precision, data->actual_type);
	//printf("len = %d\n", len);
	*size = ft_size_p(data->actual_type, , precision);
	if (ft_in_set("s", data->actual_type))
		return (ft_strndup(str, precision));
	else if (data->actual_type == 'c'|| precision <= len)
		return (ft_strdup(str));
	else if (ft_in_set("udixXp", data->actual_type))
	{
		//printf("jj\n");
		if (ft_in_set("di", data->actual_type) && str[0] == '-')
			precision++;
		if (!(ret = ft_calloc(precision + 1, sizeof(char))))
			return (0);
		ft_memset(ret, '0', precision);
		ft_memmove(ret + precision - len, str, len);
		return (ret);
	}
//	} else if (ft_in_set("p", data->actual_type))
//		return (ft_precision_ptr(data, str));
	//printf("mal%c\n", data->actual_type);
	return (0);
	//free(tmp2);
}

char	*ft_build_str(t_data *data, char *str, int *size)
{	char	*tmp;
	char	*ret;
	//printf("str = %s\n", str);
	if (!str)
		if(!(str = ft_strdup("(null)")))
			return (0);
	if (!(tmp = (ft_precision_treatment(data, str, size))))
		return (0);
	//printf("\n\nafter precision >%s<\n\n", tmp);
	if (!(ret = (ft_width_treatment(data, tmp, size))))
		return (0);
	//printf("\n\nafter width >%s<\n\n", ret);
	free(tmp);
	return (ret);
}

int		ft_printf_chr(t_data *data)//revisar size input

{
	char	*tmp;
	char	*builded;

	data->actual_type = 'c';	
	if (!(tmp = ft_calloc(2, sizeof(char))))
		return (0);
	tmp[0] = va_arg(g_args, int);
	if (!(builded = ft_build_str(data, tmp)))//
		return (0);
	//
	if (!ft_save(data, builded))
		return (0);
	free(tmp);
	free(builded);
	data->pos++;
	return (1);
	//
}

int		ft_printf_str(t_data *data)//revisar size input

{
	char	*tmp;
	char	*builded;

	data->actual_type = 's';
	tmp = va_arg(g_args, char*);
	if (!(builded = ft_build_str(data, tmp)))
		return (0);
//	printf("builded: >%s<\n", builded);
	if (!ft_save(data, builded))
		return (0);
	free (builded);
	data->pos++;
	return (1);
	//
}

int		ft_printf_uns(t_data *data)//revisar size input

{
	char			*tmp;
	char			*builded;
	unsigned int 	aux;

	data->actual_type = data->str[data->pos];
	aux = va_arg(g_args, unsigned int);
	tmp = ft_itoa_unsigned(aux, "0123456789");
	
	if (ft_strlen(tmp) == 1 && tmp[0] == '0' && data->precision == 0)
	{
		free(tmp);
		tmp = ft_strdup("");
	}
	if (data->precision >= 0)
		data->zero_flag = 0;
	if (!(builded = ft_build_str(data, tmp)))
		return (0);

	//
	if (!ft_save(data, builded))
		return (0);
	free(tmp);
	free(builded);
	data->pos++;
	return (1);
	//

}

int		ft_printf_int(t_data *data)//revisar size input

{
	char	*tmp;
	char	*builded;
	int		aux;

	data->actual_type = data->str[data->pos];
	aux = va_arg(g_args, int);
	tmp = ft_itoa(aux);
	if (ft_strlen(tmp) == 1 && tmp[0] == '0' && data->precision == 0)
	{
		free(tmp);
		tmp = ft_strdup("");
	}
	if (data->precision >= 0)
		data->zero_flag = 0;
	if (!(builded = ft_build_str(data, tmp)))
		return (0);

	
	ft_check_sign_position(builded);
	//
	if (!ft_save(data, builded))
		return (0);
	free(tmp);
	free(builded);
	data->pos++;
	return (1);
	//
}

int		ft_printf_hex(t_data *data)//revisar size input

{
	long unsigned int	num;
	char				*b;
	char				*tmp;
	char				*builded;

	data->actual_type = data->str[data->pos];
	if (data->precision >= 0)
		data->zero_flag = 0;	
	b = data->actual_type == 'x' ? ft_strdup("0123456789abcdef") :
		ft_strdup("0123456789ABCDEF");
	//printf("base: %s\n", b);
	num = va_arg(g_args, long unsigned int);
	if(!(tmp = ft_itoa_unsigned(num, b)))
		return (0);
	if (ft_strlen(tmp) == 1 && tmp[0] == '0' && data->precision == 0)
	{
		free(tmp);
		if(!(tmp = ft_strdup("")))
			return (0);
	}
	//printf("tmp >%s<\n", tmp);
	if(!(builded = ft_build_str(data, tmp)))
		return (0);
	//printf("builded >%s<\n", builded);
	if(!ft_save(data, builded))
		return (0);
	free(tmp);
	free(builded);
	data->pos++;
	return (1);
}

int		ft_printf_ptg(t_data *data)//revisar size input

{
	char	*tmp;
	char	*builded;

	data->actual_type = 'c';	
	if (!(tmp = ft_calloc(2, sizeof(char))))
		return (0);
	tmp[0] = '%';
	if (!(builded = ft_build_str(data, tmp)))//
		return (0);
	//
	if (!ft_save(data, builded))
		return (0);
	free(tmp);
	free(builded);
	data->pos++;
	return (1);
	//
}

char	*ft_check_ptr(char *str)
{
	int		i;
	int		j;
	int		k;
	char	*ret;

	i = 0;
	j = 0;
	k = -1;
	while (str[i] == ' ')
		i++;
	while (str[i + j])
		j++;
	if (!(ret = malloc(i + j + 3)))
		return (0);
	while (++k < i)
		ret[k] = ' ';;
	ret[k++] = '0';
	ret[k++] = 'x';
	while (str[i])
	{
		ret[k] = str[i];
		k++;
		i++;
	}
	free(str);
	return (ret);
}

int		ft_printf_ptr(t_data *data)//revisar size input

{
	long unsigned int		addr;
	char					*tmp;
	char					*builded;
	
	data->width -= 2;
	if (data->zero_flag) //si flag 0 trata width como precision
	{					//precision = width - 2 si widht - 2 > precision
		if (data->precision >= 0)
			data->precision = data->precision < data->width ? data->width :
				data->precision;
		data->width = 0;
	}
	addr = va_arg(g_args, long unsigned int);
	if (!(tmp = addr ? ft_itoa_unsigned(addr, "0123456789abcdef") : ft_strdup("")))
		return (0);
	data->precision = !*tmp && data->precision == -1 ? 1 : data->precision;
	data->actual_type = data->str[data->pos];
	if (!(builded = ft_build_str(data, tmp)))
		return (0);
	builded = ft_check_ptr(builded);
	if (!ft_save(data, builded))
		return (0);
	data->pos++;
	free(tmp);
	free(builded);
	return (0);
}

int		ft_printf_err(t_data *data) //revisar size input
{
	char 	*tmp;
	char	*builded;
	int		size;

	data->actual_type = 's';
	data->width--;//muy cutre
	tmp = ft_strdup("");
	size = 0;
	if (!(builded = ft_build_str(data, tmp, &size)))
		return (0);
	//
	if (!ft_save(data, builded, size))
		return (0);
	free (builded);
	return (1);
	//
}
