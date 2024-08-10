include Makefile

DEST = debug

${DEST}/ilisp.exe: $(addprefix ${DEST}/, ${OBJS})
	clang++ -o $@ -stdlib=libc++ $^

${DEST}/%.o: %.cc ${HHS}
	clang++ -O2 -o $@ -std=c++2b -stdlib=libc++ $<

clean:
	echo>${DEST}\ilisp.exe
	echo>${DEST}\a.o
	del ${DEST}\ilisp.exe
	del ${DEST}\*.o

.PHONY: clean