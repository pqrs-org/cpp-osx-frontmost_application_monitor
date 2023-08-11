all:
	xcodegen generate
	xcodebuild -configuration Release -alltargets SYMROOT="$(CURDIR)/build"
	make run

clean:
	rm -rf build

run:
	./build/Release/test
