.PHONY: build prepare tests tests-*

build:
	mkdir -p build && cd build && cmake .. && make && sudo make install
	

prepare:
	./إعداد_للتطوير


tests:
	./اختبارات/اختبر


tests-syntax:
	./اختبارات/اختبر -ق


tests-fill:
	./اختبارات/اختبر -ك


tests-renew:
	./اختبارات/اختبر -ك
