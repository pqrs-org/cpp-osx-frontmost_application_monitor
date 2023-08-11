all:
	xcodegen generate
	xcodebuild -configuration Release -alltargets SYMROOT="$(CURDIR)/build"
	make run

clean:
	rm -rf build
	rm -rf *.xcodeproj

run:
	./build/Release/test
