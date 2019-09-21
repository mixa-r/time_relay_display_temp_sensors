# time_relay_lсd_temp_sensors
Добрый день! Данный проект был переделан под свои нужды
 - Реле времени с недельный таймером на 4 канала см. https://lesson.iarduino.ru/page/rele_time/
 - Реле времени добавились кнопки и датчик температуры
 - В код внесены изменения в меню добавлено подменю температура и добавлены кнопки.
 
 Принцип работы:
 В версии v.1 программный код реле на 4 канала с возможностью установить на каждый канал свой таймер на неделю. В меню присутствует настройка даты/времени/таймера.
 
 В версии v.2 программный код реле на 4 канала с возможностью установить на каждый канал свой таймер на неделю. В меню присутствует настройка даты/времени/таймера/температуры. Также добавлены кнопки для включения/выключения реле отдельно от таймера.
Реле 1/2 включаеются и выключаются как по таймеру так и в ручном режиме.
Реле 3/4 включаются и выключаются по температуре устанавливаемой через меню на дисплее. В програмном коде есть константа гистерезиса "tgis" она отвачает за разницу при включении и выключении реле. Разобраться с кнопками не удалось, поставил кнопки с фиксацией чтобы не мучить себя долгими размышлениями.
В данном скетче используется датчики температуры DTH11. Библиотека сама определяет какой датчик DTH установлен.

 В версии v.3 доработаю показания с температурных датчиков DS18B20.
Подключены датчики DS18B20. Считав адреса датчиков. При отсутсвии датчиков на экран будет выведена информация об отсутсвии показаний пока не подключатся датчики. Также добавлены светодиоды индикция нажатия кнопок. В остальном программа соответствует v.2

 
                                        Для реализации проекта нам понадоиться:               
- Arduino Uno/Nano х 1шт.
- Дисплей LCD1602 I2C зелёный или синий x 1шт.
- Модуль - RTC (часы реального времени) x 1шт.
- Модуль - энкодер x 1шт.
- Кнопки с фиксацией х 2 шт.
- Кнопки с без фиксации х 2 шт.
- Короб монтажный (корпус) х 1 шт.
- Светодиод х 4 шт.
- Винты для крепления 
- Провода
- Монтажная плата при необходимости

                                 Для реализации проекта нам необходимо установить библиотеки:
- LiquidCrystal_I2C для работы с символьными ЖК дисплеями.
- iarduino_Encoder_tmr для работы с энкодерами через аппаратный таймер.
- iarduino_RTC для работы с модулями реального времени.
- Библиотеки EEPROM, Wire и pgmspace используемые в скетче, входят в стандартный набор Arduino IDE.
- iarduino_DHT.h для работы с датчиками температуры DTH11/22
- Для датчиков ds18b20:
    OneWire.h
    DallasTemperature.h
