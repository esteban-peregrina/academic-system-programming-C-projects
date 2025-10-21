EXEC = question4
CFLAGS = -Wall -ggdb3

all: $(EXEC)


question4: image.h image.c transf_image.h transf_image.c bruitage_multiprocess.c 
	gcc $(CFLAGS) -o question4 image.c transf_image.c bruitage_multiprocess.c

clean:
	rm -f *.o *~ *.backup

vclean:	clean
	rm -f $(EXEC)
