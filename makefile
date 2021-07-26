.PHONY: build prepare tests tests-*

build:
	mkdir -p build && cd build && cmake .. && make && sudo make install

prepare:
	python ./إعداد_للتطوير.py

prepare-tests:
	python ./إعداد_للتطوير.py اختبارات

tests:
	./اختبارات/اختبر

tests-syntax:
	./اختبارات/اختبر -ق

tests-fill:
	./اختبارات/اختبر -ك

tests-renew:
	./اختبارات/اختبر -ك
