<div dir=rtl>

The Arabic Programming Language, Alif - لغة البرمجة العربية، ألف
-----------------------------------------------------------

كيف يعمل مترجم ألف ؟
-------

مترجم ألف يقوم بترجمة لغة ألف إلى لغة سي++ مع استعمال مكتبات [بوست](https://boost.org)

طريقة ترجمة الشيفرة المصدرية للمشروع
---------

### المتطلبات

1. مترجم مناسب، مثل مترجم gcc الإصدار رقم 8 على الأقل
2. مكتبات بوست، على الأقل الإصدار ﻻقم 1.76

### استنساخ الكود لجهازك

<div dir=ltr>

```bash
git clone --depth=1 -b main --single-branch https://github.com/alifcommunity/compiler.git
cd compiler
mkdir build
cd build
```

</div>

### Windows

البناء باستخدام ___GCC (MinGW64)___.

<div dir=ltr>

```bash
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make
```

</div>

البناء باستخدام ___Microsoft build tools 2019___.

<div dir=ltr>

```bash
cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release 
nmake
```

</div>

### Linux

تنصيب مترجم GCC & GTK3

<div dir=ltr>

```
sudo apt install build-essential libgtk-3-dev
```

</div>

تنصيب مكتبات Boost +1.76

إن لم تكن موجودة بالفعل في المتسودع الرسمي لحزم توزيعة اللينوكس، قم بتنصيبها يدويا:

<div dir=ltr>

```
wget https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.gz
tar -xzf boost_1_76_0.tar.gz
cd boost_1_76_0
./bootstrap.sh
sudo ./b2 install address-model=64 variant=release link=static runtime-link=shared warnings=off --with-filesystem --with-program_options --with-system --with-locale --with-date_time --with-regex --with-nowide --with-thread 
```

</div>

بناء مترجم ألف باستخدام GCC.

<div dir=ltr>

```bash
cmake ..
make && sudo make install
```

</div>

المساهمة في تطوير اللغة
---------

قال صلى الله عليه وسلم : "يد الله مع الجماعة". بدأت اللغة كعمل فردي ومبادرة من حسن دراجة، لكن اللغة تنمو بالمساهمين والمطورين الذي يريدون رؤية اللغة العربية على رأس اللغات وأفضلهم.

### المتطلبات

1. أنت بحاجة لتوفية منطلبات البناء في الجزء الخاص ببناء المترجم، _هذا بديهي_  😅
2. على الأقل أنت بحاجة لـ "جيت" الإصدار 2.9.0 لاستعمال الخطافات، [المصدر](https://stackoverflow.com/questions/39332407/git-hooks-applying-git-config-core-hookspath)
3. قمت بتنصيب clang-format، تأتي ضمن مترجم ال clang

### استنساخ الكود وإعداده

<div dir=ltr>

```
git clone https://github.com/alifcommunity/compiler.git alif-compiler
```

```
./إعداد-للتطوير
```

</div>
  
[ ! ] تنبيه

مشروع ألف نسخة 3 هو قيد التطوير ولا يصلح حاليا للاستخدام

لكن يمكنك استعمال وتجربة ألف النسخة 2 عبر تحميله من الموقع الرسمي

[www.aliflang.org](https://aliflang.org)

بالتوفيق.

</div>
