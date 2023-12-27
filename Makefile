clean:
	rm -rf output/app

build:
	mkdir -p output
	gcc -std=gnu17 src/state.c -lcurl -g -o output/app

run:
	./output/app "http://localhost:8088" 1000