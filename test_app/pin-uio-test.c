/*
 * This application reads/writes GPIO devices with UIO.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void usage(void)
{
	printf("*argv[0] -d <UIO_DEV_FILE>\n");
	printf("    -d               UIO device file. e.g. /dev/uio0");
	return;
}

int main(int argc, char *argv[])
{
	int c;
	int fd;
	char *uiod;
	unsigned i = 0;
	unsigned icount;
	int irq_on = 1;
	int err;
	
	printf("pin UIO test.\n");
	while((c = getopt(argc, argv, "d:io:h")) != -1) {
		switch(c) {
		case 'd':
			uiod=optarg;
			break;
		case 'h':
			usage();
			return 0;
		default:
			printf("invalid option: %c\n", (char)c);
			usage();
			return -1;
		}
		
	}

	/* Open the UIO device file */
	fd = open(uiod, O_RDWR);
	if (fd < 1) {
		perror(argv[0]);
		printf("Invalid UIO device file:%s.\n", uiod);
		usage();
		return -1;
	}

	for(i = 0; ; ++i) {
		/* Print out a message, for debugging. */
        	if (i == 0)
            		fprintf(stderr, "Started uio test driver.\n");
        	else
            		fprintf(stderr, "Interrupts: %d\n", icount);

		/* enable IRQ, trigger the irqcontrol of driver */
		write(fd, &irq_on, sizeof(irq_on));
       		/* Here we got an interrupt from the
           	device. Do something to it. */
		err = read(fd, &icount, 4);
		if (err != 4) {
			perror("uio read:");
			break;
		}
	}

	return 0;
}


