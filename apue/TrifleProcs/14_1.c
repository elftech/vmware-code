//This program should be complied with 3_clr_fl.c 3_5.c
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char	buf[500000];

int
main(void)
{
	int		ntowrite, nwrite;
	char	*ptr;

	ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stderr, "read %d bytes\n", ntowrite);

	set_fl(STDOUT_FILENO, O_NONBLOCK);	/* set nonblocking */

	ptr = buf;
	while (ntowrite > 0) {
		errno = 0;
		nwrite = write(STDOUT_FILENO, ptr, ntowrite);
		fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);

		if (nwrite > 0) {
			ptr += nwrite;
			ntowrite -= nwrite;
		}
	}

	clr_fl(STDOUT_FILENO, O_NONBLOCK);	/* clear nonblocking */

	exit(0);
}
