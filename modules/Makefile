all:
	for i in */Makefile; do make -C $${i%/Makefile}; done

clean:
	for i in */Makefile; do make -C $${i%/Makefile} clean; done