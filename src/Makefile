CC=g++
CFLAGS=-Wall -Werror -Wextra -std=c++17
LIBS=-lgtest
LEAKS=leaks --atExit -- build/tests
BUILD=build

OS=$(shell uname -s)
ifeq ($(OS), Linux)
	LIBS+= -lpthread
	LEAKS=valgrind build/tests
endif

all: install tests

install: clean
	@qmake CONFIG+=release -o $(BUILD)/Makefile View/3DViewer.pro
	@cd $(BUILD); make
	@rm -rf $(BUILD)/*.o
	@rm -rf $(BUILD)/*.cc
	@rm -rf $(BUILD)/*.cpp
	@rm -rf $(BUILD)/*.h
	@rm -rf $(BUILD)/Makefile
	open $(BUILD)

uninstall:
	@rm -rf $(BUILD)/3DViewer.app

clean:
	@rm -Rf *.o *.gcno *.gcda *.info report build/ valgrind_output .clang-format ./viewer_tar test_report
	@rm -Rf Viewer.html

dvi: clean
	makeinfo -o Viewer.html --html --no-warn --force 
	open Viewer.html/index.html
# cat ./doc.texi

dist: clean
	@mkdir -p ./viewer_tar
	@tar -cvzf ./viewer_tar/View.tgz Makefile Controller/controller.h Model/s21_object.* Test/* View/*.cc View/*.h View/*.ui View/3DViewer.pro *.texi

tests: test_build
	@build/tests

clang:
	@cp ../materials/linters/.clang-format .
	@clang-format -i Controller/*.h Model/s21_object.* Test/*.cc View/*.cc View/*.h
	@clang-format -n Controller/*.h Model/s21_object.* Test/*.cc View/*.cc View/*.h

leaks: test_build
	$(LEAKS)

check:
	@cppcheck --language=c++ Controller/*.h Model/s21_object.* Test/*.cc View/*.cc View/*.h

test_build: clean
	@mkdir build
	@g++ Model/*.cc Test/tests.cc -lgtest -o build/tests
