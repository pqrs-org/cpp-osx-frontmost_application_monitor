all:
	xcodegen generate
	xcodebuild -configuration Release -alltargets SYMROOT="$(CURDIR)/build"

clean:
	rm -rf build
	rm -rf *.xcodeproj

run:
	./build/Release/example
