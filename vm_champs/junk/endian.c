

#include <stdio.h>

typedef union	u_end
{
	int			x;
	char		i;
	char		j;
	char		k;
	char		l;
}				t_end;

int		main(void)
{
	t_end	e;

	e.x = 1;
	printf("i: %d\n", e.i);
	printf("j: %d\n", e.j);
	printf("k: %d\n", e.k);
	printf("l: %d\n", e.l);

	printf("%p\n", &e.x);
	char	*s;
	s = (char *)&e.x;
	printf("%p\n", s);
	printf("%p\n", s + 1);
	printf("%d\n", *(s));
	printf("%d\n", *(s + 1));
	return (0);
}
