<div dir=rtl>

The Alif Arabic Programming Language - لغة البرمجة العربية ألف
-----------------------------------------------------------

كيف يعمل مترجم ألف ؟
-------

مترجم ألف يقوم بترجمة لغة ألف إلى لغة سي++ مع استعمال مكتبات 
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
  
Windows - GCC (MinGW64).

```bash
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make
```

Windows - Microsoft build tools 2019.

```bash
cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release 
nmake
```

Linux - Install Boost +1.76
```
wget https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.gz
tar -xzf boost_1_76_0.tar.gz
cd boost_1_76_0
./bootstrap.sh
sudo ./b2 install address-model=64 variant=release link=static runtime-link=shared warnings=off --with-filesystem --with-program_options --with-system --with-locale --with-date_time --with-regex --with-nowide --with-thread 
```

Linux - GCC.

```bash
cmake ..
make && sudo make install
```

</div>

[ ! ] تنبيه

مشروع ألف نسخة 3 هو قيد التطوير ولا يصلح حاليا للاستخدام

لكن يمكنك استعمال وتجربة ألف النسخة 2 عبر تحميله من الموقع الرسمي

[www.aliflang.org](https://aliflang.org)

بالتوفيق.

</div>
