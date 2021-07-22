.PHONY: tests tests-syntax tests-fill tests-renew build

build:
	mkdir -p build && cd build && cmake .. && make && sudo make install
	
tests:
	./اختبارات/اختبر

tests-syntax:
	./اختبارات/اختبر -ق

tests-fill:
	./اختبارات/اختبر -ك

tests-renew:
	./اختبارات/اختبر -ك
