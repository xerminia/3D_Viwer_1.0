CC = gcc
CFLAGS = -Wall -Werror -Wextra
FLAG = --coverage
OS := $(shell uname -s)

all: clean install test

build:
	mkdir build
	cd ./build/ && qmake6 ../ && make && mv kek.app 3DViewer_v1.0.app

install: build
	mkdir $(HOME)/Desktop/3DViewer_v1.0/
	cp -rf build/3DViewer_v1.0.app $(HOME)/Desktop/3DViewer_v1.0/
	make clean

uninstall:
	rm -rf $(HOME)/Desktop/3DViewer_v1.0/

dvi:
	open dvi.html

dist:
	rm -rf 3DViewer_v1.0
	mkdir 3DViewer_v1.0
	mkdir 3DViewer_v1.0/src
	cp Makefile *.c *.h *.cpp *.ui *.pro *.pro.user *.html 3DViewer_v1.0/src/
	cp -rf 3rdParty 3DViewer_v1.0/src/
	tar -cvzf 3DViewer_v1.0.tar.gz 3DViewer_v1.0
	mv 3DViewer_v1.0.tar.gz $(HOME)/Desktop/
	rm -rf 3DViewer_v1.0/

3d.o:
	$(CC) $(CFLAGS) -c 3d.c -o 3d.o
	
test.o:
	$(CC) $(CFLAGS) -c test.c -o test.o

test: 3d.o test.o
ifeq ($(OS), Darwin)
	$(CC) test.o 3d.o -o tests -lcheck
	./tests
else
	$(CC) test.o calc.o -o tests -lcheck -lsubunit -lrt -lpthread -lm
	./tests
endif

gcov: clean
ifeq ($(OS), Darwin)
	$(CC) $(FLAG) 3d.c test.c -o test -lcheck
else
	$(CC) $(FLAG) s21_matrix.c test.c -o test -lcheck -lsubunit -lrt -lpthread -lm
endif

gcov_report: clean gcov
	./test
	lcov -t “test” -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html

clean:
	rm -rf *.o  *.gc* report *.a tests test test.info build

check: test
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extensions=c *.c
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	CK_FORK=no leaks --atExit -- ./tests
	rm -rf CPPLINT.cfg
