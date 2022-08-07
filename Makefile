PREFIX=$(PWD)/external
SYSTEMC_HOME=$(PREFIX)

REDHATFLAGS=-D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fasynchronous-unwind-tables -fexceptions -fpie -Wl,-pie -fstack-clash-protection -fstack-protector-all -fstack-protector-strong -g -grecord-gcc-switches -fcf-protection -O2 -pipe -Wall -Werror=format-security -Wl,-z,defs
CPPVERSIONFLAGS=-DSC_CPLUSPLUS="201402L" -std="c++14"
SYSTEMCFLAGS=-I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib
SCAFLAGS=-I$(PWD)/inc -I$(PREFIX) -I$(PREFIX)/include

all: unit

unit: clean
	mkdir -p objdir
	g++  $(REDHATFLAGS) $(CPPVERSIONFLAGS) $(SYSTEMCFLAGS) $(SCAFLAGS) -DRUN_UNIT_TESTS tst/Utils.cpp tst/main.cpp -lsystemc -lm -o ./objdir/main.o -pg
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./main.o -s

fmt:
	clang-format --style="{BasedOnStyle: google}"  -i inc/* src/* tst/* validate/*.cpp

validate: sv cpp
	diff $(TEST_NAME)_sv.result $(TEST_NAME)_cpp.result && echo "Validation passed for $(TEST_NAME)" >> ./validation.result || echo "Validation failed for $(TEST_NAME)" >> ./validation.result

sv: clean
	mkdir -p objdir
	cd objdir \
		&& vcs -R -sverilog -ntb_opts dep_check +define+$(TEST_NAME) ../validate/validate.sv

cpp: clean
	mkdir -p objdir
	g++  $(REDHATFLAGS) $(CPPVERSIONFLAGS) $(SYSTEMCFLAGS) $(SCAFLAGS) -D$(TEST_NAME) validate/validate.cpp -lsystemc -lm -o ./objdir/main.o -pg
	export LD_LIBRARY_PATH=$(SYSTEMC_HOME)/lib && cd objdir && ./main.o -s

tidy:
	clang-tidy --header-filter=.* --system-headers -p compile_commands.json src/main.cpp

clean:
	rm -rf objdir systemc* spdlog*

# -----------------------------------------------------------------------------
#  Dependency installation
# -----------------------------------------------------------------------------
deps: spdlog doctest systemc

spdlog:
	rm -rf objdir
	mkdir -p objdir
	cd objdir \
		&& wget https://github.com/gabime/spdlog/archive/refs/tags/v1.9.2.tar.gz \
		&& tar xf *
	mkdir -p $(PREFIX)
	cp -r objdir/spdlog-1.9.2/include/spdlog $(PREFIX)/.

doctest:
	wget https://github.com/doctest/doctest/releases/download/v2.4.9/doctest.h
	mkdir -p $(PREFIX)/doctest
	mv doctest.h $(PREFIX)/doctest/.

systemc:
	mkdir -p $(PREFIX)
	wget https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz
	tar xf systemc-2.3.3.tar.gz
	mkdir -p systemc-2.3.3/objdir
	cd systemc-2.3.3/objdir \
		&& ../configure CXXFLAGS="-std=c++14" --prefix=$(PREFIX) --with-unix-layout \
		&& make -j 4 \
		&& make install \
		;
	rm -rf systemc-2.3.3.tar.gz
	rm -rf systemc-2.3.3
