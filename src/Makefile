CC=g++
CFLAGS=-Wall -Werror -Wextra -std=c++17
LIBS=-lgtest
LEAKS=leaks --atExit -- ./tests
BUILD=build

OS=$(shell uname -s)
ifeq ($(OS), Linux)
	LIBS+= -lpthread
	LEAKS=valgrind ./tests
endif

all: install dvi

install:
	@qmake CONFIG+=release -o $(BUILD)/Makefile 3DViewer/3DViewer.pro
	@cd $(BUILD); make
	@rm -rf $(BUILD)/*.o
	@rm -rf $(BUILD)/*.cc
	@rm -rf $(BUILD)/*.cpp
	@rm -rf $(BUILD)/*.h
	@rm -rf $(BUILD)/Makefile
	open $(BUILD)

uninstall:
	@rm -rf $(BUILD)

clean:
	@rm -Rf *.o *.gcno *.gcda *.info report tests valgrind_output .clang-format ./viewer_tar test_report
	@rm -Rf Viewer.html

dvi: clean
	makeinfo -o Viewer.html --html --no-warn --force ./doc.texi
	open Viewer.html/index.html

dist: clean
	@mkdir -p ./viewer_tar
	@tar -cvzf ./viewer_tar/3DViewer.tgz Makefile controller.h s21_object.* tests.cc 3DViewer/*.cc 3DViewer/*.h 3DViewer/*.ui 3DViewer/3DViewer.pro *.texi *.obj

tests: test_build
	./tests

clang:
	@cp ../materials/linters/.clang-format .
	@clang-format -i *.cc *.h 3DViewer/*.cc 3DViewer/*.h
	@clang-format -n *.cc *.h 3DViewer/*.cc 3DViewer/*.h

leaks: test_build
	$(LEAKS)

check:
	@cppcheck --language=c++ *.h *.cc 3DViewer/*.cc 3DViewer/*.h

test_build:
	@g++ s21_object.cc tests.cc -lgtest -o tests
