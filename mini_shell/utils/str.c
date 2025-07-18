#include "../minishell.h"

char	*string_concatation(char **str)
{
	char	*result;
	int		total_byte;
	int		i;
	
	i = 0;
	total_byte = 0;
	while(str[i]){
		total_byte+= ft_strlen(str[i]);
		i++;
	}
	result = malloc(total_byte + 1);
	i = 0;
	total_byte = 0;
	if(!result)
		return (NULL);
	while(str[i]){
		ft_memcpy(result + total_byte, str[i], ft_strlen(str[i]));
		total_byte += ft_strlen(str[i]);
		i++;
	}
	ft_memcpy(result + total_byte, "\0", 1);
	return (result);
}