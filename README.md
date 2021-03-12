The Alif Arabic Programming Language - لغة البرمجة العربية ألف
-----------------------------------------------------------

كيف يعمل مترجم ألف ؟
-------

مترجم ألف يقوم بترجمة لغة ألف إلى لغة سي++ مع إستعمال مكتبات 
[بوست](https://boost.org)

طريقة ترجمة الشيفرة المصدرية للمشروع
---------

Windows - Microsoft build tools 2019.
```
$ mkdir build 
$ cd build 
$ cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release 
$ nmake
```

Linux - GCC.
```
$ mkdir build 
$ cd build 
$ cmake .. 
$ make
$ sudo make install
```

[ ! ] تنبيه

مشروع ألف نسخة 3 هو قيد التطوير ولا يصلح حاليا للإستخدام

لكن يمكنك ستعمال وتجربة ألف نسخة 2 عبر تحميله من الموقع الرسمي

[www.aliflang.org](https://aliflang.org)

بالتوفيق،
