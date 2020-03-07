keeloq: keeloq.c
	gcc -o keeloq keeloq.c

install: keeloq
	cp ./keeloq /usr/local/bin
	chmod a+x /usr/local/bin/keeloq
	chmod a+r /usr/local/bin/keeloq