include Makefile

debug/ilisp: $(addprefix debug/, ${OBJS})
	clang++ -o $@ -stdlib=libc++ $^

debug/%.o: %.cc ${HHS}
	clang++ -O2 -o $@ -std=c++2b -stdlib=libc++ $<

clean:
	touch debug/ilisp debug/a.o
	rm debug/ilisp debug/*.o

.PHONY: clean