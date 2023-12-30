clean:
	rm -rf output/app

build:
	mkdir -p output
	gcc -std=gnu17 src/state.c -o output/app

build-dev:
	mkdir -p output
	gcc -std=gnu17 src/state.c -g -o output/app

run:
	./output/app localhost 55554 1

run-dev:
	gdb --args ./output/app localhost 55554 1