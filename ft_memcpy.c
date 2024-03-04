#include "MemoryManager.h"
//good
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned int	i;
	unsigned char	*ptr_dest;
	unsigned char	*ptr_src;

	ptr_src = (unsigned char *)src;
	ptr_dest = (unsigned char *)dest;
	i = 0;
	if (dest == src)
		return (dest);
	while (i < n)
	{
		ptr_dest[i] = ptr_src[i];
		i++;
	}
	return (dest);
}