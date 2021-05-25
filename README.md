<div dir=rtl>

The Alif Arabic Programming Language - لغة البرمجة العربية ألف
-----------------------------------------------------------

كيف يعمل مترجم ألف ؟
-------

مترجم ألف يقوم بترجمة لغة ألف إلى لغة سي++ مع إستعمال مكتبات 
[بوست](https://boost.org)

طريقة ترجمة الشيفرة المصدرية للمشروع
---------

<div dir=ltr>

```bash
git clone --depth=1 https://github.com/alifcommunity/compiler.git
cd compiler
mkdir build
cd build
```
  
Windows - GCC (TDM/MinGW).

```bash
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
nmake
```

Windows - Microsoft build tools 2019.

```bash
cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release 
nmake
```

Linux - GCC.

```bash
cmake ..
make && sudo make install
```

</div>

[ ! ] تنبيه

مشروع ألف نسخة 3 هو قيد التطوير ولا يصلح حاليا للإستخدام

لكن يمكنك ستعمال وتجربة ألف نسخة 2 عبر تحميله من الموقع الرسمي

[www.aliflang.org](https://aliflang.org)

بالتوفيق،

</div>
