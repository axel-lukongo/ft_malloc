#include "MemoryManager.h"

void	*ft_memset( void *pointer, int value, size_t count )
{
	unsigned int	i;
	char			*ptr;

	ptr = pointer;
	i = 0;
	while (i < count)
	{
		ptr[i] = (unsigned char)value;
		i++;
	}
	return (ptr);
}
