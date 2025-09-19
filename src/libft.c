#include "traceroute.h"

size_t ft_strlen(const char *s)
{
	unsigned long i = 0;

	while (s[i] != '\0'){
		i++;
	}

	return i;
}

int	ft_strncmp(const char *s1, const char *s2, const size_t n)
{
	unsigned int i = 0;

	while ((*s1 || *s2) && i < n)
	{
		if (*s1 != *s2){
			return (unsigned char)*s1 - (unsigned char)*s2;
		}

		s1++;
		s2++;
		i++;
	}

	return 0;
}

size_t ft_strncpy(char *dest, const char *src, const size_t n)
{
	size_t i = 0;
	const size_t src_len = ft_strlen(src);

	if (!n){
		return src_len;
	}

	while (src[i] != '\0' && i < n - 1)
	{
		dest[i] = src[i];
		i++;
	}

	if (n < src_len){
		dest[n - 1] = '\0';
	} else if (n != 0){
		dest[i] = '\0';
	}

	return src_len;
}

void *ft_memset(void *b, const int c, size_t len)
{
	unsigned char* ptr = b;

	while (len > 0) {
		*ptr++ = (unsigned char) c;
		len--;
	}
	return b;
}