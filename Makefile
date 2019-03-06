.PHONY: test
test:
	prove

.NOTMAIN: clean
.PHONY:   clean
clean:
	find ./t -name '*.cpp.exe' -o -name '*.c.exe' -delete
