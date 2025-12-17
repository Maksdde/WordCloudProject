Краткое описание проекта:
> WordCloud — утилита командной строки для генерации облака слов 6 различных форм.
  Программа автоматически фильтрует текст, убирая слова из одной буквы. Цвета для слов выбираются случайным образом.

Для работы нужно:
  - MinGW
  - Qt 6.5.3 (путь: E:\Qt\6.5.3\mingw_64)
  - Google Test v1.14.0

Сборка программы:
> Для корректной сборки программы необходимо установить библиотеку QT 
  в указанном выше месте или изменить путь до папки в <build_mingw.bat>, а также установить библиотеку Google Test.
  После этого запустить программу, исполняемые файл <WordCloud.exe> и <WordCloudTests.exe> будут в папке `build/`.

Тестирование:
> Тесты реализованы через библиотеку Gtest. Для корректной работы необходимо скачать Google Test v1.14.0 и распаковать 
  в папку `google/` в корне проекта.

Утилита для командной строки:
> WordCloud.exe <input.txt>
  При отсутствии иных вводных, название изображения будет <output.jpg>, размер 600*600 пикселей, форма фигуры — спираль.

Параметры:
  - <input.txt>              — текстовый файл
  - -o, --output <file>      — выходное изображение (jpg)
  - -s, --shape <shape>      — форма облака (по умолчанию spiral)
  - -W, --width <pixels>     — ширина изображения (по умолчанию: 800)
  - -H, --height <pixels>    — высота изображения (по умолчанию: 600)

Формы:
  - spiral     — спираль
  - circle     — круг
  - square     — квадрат
  - triangle   — треугольник
  - heart      — сердце
  - star       — звезда

Примеры тестов с уже готовым файлом text.txt, запускаемые из папки build:
WordCloud.exe ..\text.txt -o spiral.test -s sPIRAL
WordCloud.exe ..\text.txt -o circle.what -s CIRCLe
WordCloud.exe ..\text.txt -o square.png -s sQuare
WordCloud.exe ..\text.txt -o triangle.JPG -s trianglE
WordCloud.exe ..\text.txt -o heart -s hEARt -W 400 -H 400
WordCloud.exe ..\text.txt -o star.jpg -s stAR -W 800 -H 600




