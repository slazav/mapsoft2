all:
	sh -e -c 'for i in */Makefile; do make -C $${i%/Makefile}; done'

clean:
	sh -e -c 'for i in */Makefile; do make -C $${i%/Makefile} clean; done'