include Makefile

DEST = debug

${DEST}/ilisp: $(addprefix ${DEST}/, ${OBJS})
	clang++ -o $@ -stdlib=libc++ $^

${DEST}/%.o: %.cc ${HHS}
	clang++ -O2 -o $@ -std=c++2b -stdlib=libc++ $<

clean:
	touch ${DEST}/ilisp ${DEST}/a.o
	rm ${DEST}/ilisp ${DEST}/*.o

.PHONY: clean