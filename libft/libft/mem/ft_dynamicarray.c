#include "libft.h"

void	*ft_dynamicarray(void *src, size_t len)
{
	void	*fresh;

	if (!(fresh = (void *)malloc((sizeof(void *)) * (len * 2))))
		return (NULL);
	fresh = ft_memcpy(fresh, src, len);
	ft_memset(fresh + len, 0, len);
	return (fresh);
}
