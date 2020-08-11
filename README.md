The Alif Arabic Programming Language - لغة البرمجة العربية ألف
-----------------------------------------------------------

كيف يعمل مترجم ألف ؟
-------

مترجم ألف يقوم بترجمة لغة ألف إلى لغة سي++ مع إستعمال مكتبات [وكس ويدجيتز](https://github.com/wxWidgets)

طريقة الأستعمال
-------

```
$ alif [source.alif / source.aliflib] [output binary]  [logfile]
```

ويندوز
---------

```
$ alif foo.alif bar.exe log.txt
```

لينكس
---------

```
$ alif foo.alif bar log.txt
```

ماك أوس
---------

```
$ alif foo.alif bar.app log.txt
```

طريقة ترجمة الشيفرة المصدرية للمشروع
---------

1 - تثبيث [ألف ستوديو](https://www.aliflang.org/download)

2 - تحميل أخر شيفرة مصدرية (Master)

3 - ترجمة

Linux
```
g++ -m64 -std=c++11 -Wl,--as-needed -finput-charset=utf-8 -Wall -fexceptions -O2 -Os -s -DLINUX -Wl,--gc-sections -Wl,--hash-style=both -Wl,-Bstatic -static -static-libgcc -static-libstdc++ -Wunused-but-set-variable -I"/usr/local/include/aliflibwx" -I"Src" "Src/alif.cpp" -o "alif"
```

Windows
```
g++ -Wl,-subsystem,console -mconsole -Wno-unused -m32 -std=c++11 -finput-charset=utf-8 -W -Wall -static-libgcc -static-libstdc++ -fexceptions -c "Src\alif.cpp" -o "alif.o" -I "Src" -I "c:\Alif\include"

g++ -Wl,-subsystem,console -mconsole -m32 -std=c++11 -finput-charset=utf-8 -Wall -static-libgcc -static-libstdc++ -fexceptions -s -I "c:\Alif\include" -o "alif.exe" "alif.o"
```

(جاري كتابة سكيبت ترجمة وبناء cmake، قريبا إن شاء الله)

بالتوفيق،
