TARGETS = hello hello-func

all: $(TARGETS)
.PHONY: all

$(TARGETS): %: %.bpf.o 

# example usage:
# make hello
# make hello-func
%.bpf.o: %.bpf.c
	clang \
	    -target bpf \
	    -I/usr/include/$(shell uname -m)-linux-gnu \
	    -g \
	    -O2 -o $@ -c $<

clean: 
	- rm *.bpf.o
	- rm -f /sys/fs/bpf/hello 
	- rm -f /sys/fs/bpf/hello-func

