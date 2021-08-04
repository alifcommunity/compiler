.PHONY: build prepare tests tests-*

ifeq ($(OS),Windows_NT)
build:
	rd /s /q build && mkdir build && cd build && cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release && mingw32-make && mingw32-make install
else
build:
	mkdir -p build && cd build && cmake .. && make && sudo make install
endif

prepare:
	python ./إعداد_للتطوير.py

prepare-tests:
	python ./إعداد_للتطوير.py اختبارات

tests:
	python ./اختبارات/اختبر

tests-syntax:
	python ./اختبارات/اختبر -ق

tests-fill:
	python ./اختبارات/اختبر -ك

tests-renew:
	python ./اختبارات/اختبر -ك
