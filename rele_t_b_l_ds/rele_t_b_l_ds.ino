#include <avr/pgmspace.h>                                                                     //  Подключаем библиотеку для работы с PROGMEM Arduino для хранения символов дисплея
#include <EEPROM.h>                                                                           //  Подключаем библиотеку для работы с EEPROM  Arduino для хранения значений таймеров
#include <Wire.h>                                                                             //  Подключаем библиотеку для работы с шиной I2C
#include <LiquidCrystal_I2C.h>                                                                //  Подключаем библиотеку для работы с LCD дисплеем по шине I2C
#include <iarduino_Encoder_tmr.h>                                                             //  Подключаем библиотеку iarduino_Encoder_tmr для работы с энкодерами через аппаратный таймер
#include <iarduino_RTC.h>                                                                     //  Подключаем библиотеку iarduino_RTC для работы с часами реального времени
                                                                                              //
//   ОБЪЯВЛЯЕМ КОНСТАНТЫ КОТОРЫЕ МОЖНО РЕДАКТИРОВАТЬ:                                         //
     const uint8_t   pinButton1   =             3;
     const uint8_t   pinButton2   =             11;
     const uint8_t   pinButton3   =             12;
     const uint8_t   pinButton4   =             2;
     
     const uint8_t   pinEncA     =              7;                                            //  Определяем константу с указанием № вывода Arduino к которому подключён вывод «A» энкодера
     const uint8_t   pinEncB     =              8;                                            //  Определяем константу с указанием № вывода Arduino к которому подключён вывод «B» энкодера
     const uint8_t   pinEncBTN   =              4;                                            //  Определяем константу с указанием № вывода Arduino к которому подключён вывод «S» энкодера (кнопка)
     const uint8_t   pinChanel_1 =              5;                                            //  Определяем константу с указанием № вывода Arduino который будет являеться выводом 1 канала (указываются только выводы с ШИМ, кроме выводов используемых 2 таймером Arduino)
     const uint8_t   pinChanel_2 =              6;                                            //  Определяем константу с указанием № вывода Arduino который будет являеться выводом 2 канала (указываются только выводы с ШИМ, кроме выводов используемых 2 таймером Arduino)
     const uint8_t   pinChanel_3 =              9;                                            //  Определяем константу с указанием № вывода Arduino который будет являеться выводом 3 канала (указываются только выводы с ШИМ, кроме выводов используемых 2 таймером Arduino)
     const uint8_t   pinChanel_4 =              10;                                           //  Определяем константу с указанием № вывода Arduino который будет являеться выводом 4 канала (указываются только выводы с ШИМ, кроме выводов используемых 2 таймером Arduino)
     const uint8_t   maxTimers   =              21;                                           //  Определяем константу с указанием максимального количества таймеров. Число не должно превышать количество байт EEPROM/8 (для Arduino Uno максимальное значение 128)
//ОБЪЯВЛЯЕМ ПЕРЕМЕННЫЕ ТЕМПЕРАТУРЫ ПО КОТОРЫМ БУДЕТ СРАБАТЫВАТЬ РЕЛЕ
     int tgis = 5;                                                                                                //
//НАСТРОЙКА ДАТЧИКОВ ТЕМПЕРАТУРЫ DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>
 
OneWire oneWire(A0); // вход датчиков 18b20
DallasTemperature ds(&oneWire);

DeviceAddress sensor1 = { 0x28, 0x43, 0xBA, 0xCF, 0x07, 0x00, 0x00, 0x19 }; // адрес датчика DS18B20 280054B604000092 
DeviceAddress sensor2 = { 0x28, 0x14, 0x04, 0xCF, 0x07, 0x00, 0x00, 0x5F }; 


//   ОБЪЯВЛЯЕМ КОНСТАНТЫ И ПЕРЕМЕННЫЕ НЕОБХОДИМЫЕ ДЛЯ РАБОТЫ СКЕТЧА:                          //
     const byte      rusMem[38][8] PROGMEM =    {                                             //  Определяем массив в области памяти программ, каждый элемент которого является матрицей (еще одим массивом) представления символа на дисплее
                     {31,16,16,30,17,17,30, 0}, { 0, 0,30,17,30,17,30, 0},                    //  Б,  в,  № матрицы символа в массиве: 00, 01
                     {31,16,16,16,16,16,16, 0}, { 0, 0,30,16,16,16,16, 0},                    //  Г,  г,  № матрицы символа в массиве: 02, 03
                     { 6,10,10,10,10,10,31,17}, {10,10,14, 2, 2, 0, 0, 0},                    //  Д,  4,  № матрицы символа в массиве: 04, 05
                     {17,17,17,19,21,25,17, 0}, { 0, 0,17,19,21,25,17, 0},                    //  И,  и,  № матрицы символа в массиве: 06, 07
                     {21,17,17,19,21,25,17, 0}, { 10,4,17,19,21,25,17, 0},                    //  Й,  й,  № матрицы символа в массиве: 08, 09
                     { 0, 0,18,20,24,20,18, 0}, { 0, 0,17,27,21,17,17, 0},                    //  к,  м,  № матрицы символа в массиве: 10, 11
                     { 7, 9, 9, 9, 9, 9,17, 0}, { 0, 0, 7, 9, 9, 9,17, 0},                    //  Л,  л,  № матрицы символа в массиве: 12, 13
                     {31,17,17,17,17,17,17, 0}, { 0, 0,17,17,31,17,17, 0},                    //  П   н,  № матрицы символа в массиве: 14, 15
                     {17,17,17,15, 1,17,14, 0}, { 0, 0,31, 4, 4, 4, 4, 0},                    //  У,  т,  № матрицы символа в массиве: 16, 17
                     {17,17,17,15, 1, 1, 1, 0}, { 0, 0,17,17,15, 1, 1, 0},                    //  Ч,  ч,  № матрицы символа в массиве: 18, 19
                     {17,17,17,29,19,19,29, 0}, { 0, 0,17,17,29,19,29, 0},                    //  Ы,  ы,  № матрицы символа в массиве: 20, 21
                     {16,16,16,30,17,17,30, 0}, { 0, 0, 0, 0, 0, 0,21, 0},                    //  Ь, ..., № матрицы символа в массиве: 22, 23
                     {18,21,21,29,21,21,18, 0}, { 0, 0,18,21,29,21,18, 0},                    //  Ю,  ю,  № матрицы символа в массиве: 24, 25
                     {15,17,17,15, 5, 9,17, 0}, { 0, 0,15,17,15, 5, 9, 0},                    //  Я,  я,  № матрицы символа в массиве: 26, 27
                     {31, 4,31, 0,31,16,31, 0}, {16,31,16, 0,10,21,31, 0},                    //  ПН, ВТ  № матрицы символа в массиве: 28, 29
                     {28,20,31, 0,17,17,31, 0}, {16,31,16, 0,31, 4,28, 0},                    //  СР, ЧТ  № матрицы символа в массиве: 30, 31
                     {16,31,16, 0,31,16,31, 0}, {23,21,31, 0,17,17,31, 0},                    //  ПТ, СБ  № матрицы символа в массиве: 32, 33
                     {17,17,31, 0,10,21,31, 0}, { 4,12, 4, 4,14, 0, 0, 0},                    //  ВС, 1   № матрицы символа в массиве: 34, 35
                     {14, 2,14, 8,14, 0, 0, 0}, {14, 2,14, 2,14, 0, 0, 0}};                   //  2,  3   № матрицы символа в массиве: 36, 37
     uint8_t         valArray[7] =              {0,0,0,0,0,0,0};                              //  Определяем массив элементы которого будут хранить различную информацию в зависимости от режима
     char            valChar[5] =               "    ";                                       //  Определяем массив символов (строку) информация которой будет отображаться на дисплее мигая
     uint8_t         valMode =                  0;                                            //  Определяем переменную для хранения текущего режима (например: режим 31 - установка времени)
     uint8_t         valSubMode =               0;                                            //  Определяем переменную для хранения текущего подрежима (например: режим 31, подрежим 0 - установка часов, подрежим 1 - установка минут, подрежим 2 - установка секунд)
     uint8_t         valTimerNum =              0;                                            //  Определяем переменную для хранения номера выбранного таймера (от 0 до maxTimers-1)
     bool            flgDisplayUpdate =         1;                                            //  Определяем флаговую переменную, установка которой будет сигнализировать о необходимости обновления дисплея
     void            funcEncoderRead           (void);                                        //  Объявляем функцию в которой будут выполняться действия зависящие от состояния энкодера и режима
     void            funcDisplayUpdate         (void);                                        //  Объявляем функцию в которой будет обновляться информация дисплея в зависимости от режима, подрежима, выбранного таймера и значений массива valArray
     void            funcSetPWM                (void);                                        //  Объявляем функцию которая будет устанавливать сигналы ШИМ на каналах если текущее время совпало с временем таймеров
     void            funcSetChars              (uint8_t=255,uint8_t=255,uint8_t=255,uint8_t=255,uint8_t=255,uint8_t=255,uint8_t=255); // Объявляем функцию записывающую до 7 символов из массива rusMem по номерам его элементов в область CGRAM дисплея
     uint8_t         funcReadTimer             (uint8_t=0, uint8_t=0);                        //  Объявляем функцию для чтения одного из параметров таймера (№ таймера, № параметра)
     void            funcSaveTimer             (uint8_t=0, uint8_t=0, uint8_t=0);             //  Объявляем функцию для записи одного из параметров таймера (№ таймера, № параметра, значение параметра)
     uint8_t         funcFindTimer             (void);                                        //  Объявляем функцию для поиска № следующего свободного (не установленного) таймера
     bool            funcTestTimer             (void);                                        //  Объявляем функцию для проверки соответствия данных в ячейках EEPROM Arduino значениям таймеров (если скетч запускается впервые, то данные в ячейках не будут соответствовать значениям таймеров и можно будет подготовить EEPROM к первому запуску скетча)
LiquidCrystal_I2C    lcd                       (0x27,16,2);                                   //  Объявляем объект lcd  для работы с дисплеем указывая (адрес I2C = 0x27, количество столбцов = 16, количество строк = 2)
iarduino_Encoder_tmr enc                       (pinEncA,pinEncB);                             //  Объявляем объект enc  для работы с энкодером указывая (№ вывода A, № вывода B)
iarduino_RTC         time                      (RTC_DS1307);                                  //  Объявляем объект time для работы с часами RTC указывая (тип модуля)
#define              encPRESS 2                                                               //  Определяем константу для удобочитаемости при определении состояния энкодера
                                                                                              //
void setup(){                                                                                 //
     ds.begin();                                                                              //  Инициализация датчиков температуры
     enc.begin();                                                                             //  Инициируем работу с энкодером
     time.begin();                                                                            //  Инициируем работу с RTC модулем
     Serial.begin(9600);
     
     pinMode(pinButton1,   INPUT_PULLUP);
     pinMode(pinButton2,   INPUT_PULLUP);
     pinMode(pinButton3,   INPUT_PULLUP);
     pinMode(pinButton4,   INPUT_PULLUP);
     
     pinMode(pinEncBTN,   INPUT_PULLUP);                                                             //  Переводим вывод с кнопкой энкодера в режим входа
     pinMode(pinChanel_1, OUTPUT);                                                            //  Переводим вывод 1 канала в режим выхода
     pinMode(pinChanel_2, OUTPUT);                                                            //  Переводим вывод 1 канала в режим выхода
     pinMode(pinChanel_3, OUTPUT);                                                            //  Переводим вывод 1 канала в режим выхода
     pinMode(pinChanel_4, OUTPUT);                                                            //  Переводим вывод 1 канала в режим выхода
     lcd.init();                                                                              //  Инициируем работу с LCD дисплеем
     lcd.backlight();                                                                         //  Включаем подсветку LCD дисплея
     funcSetChars(12,6);                                                                      //  Загружаем элементы 12 и 6 массива rusMem в память CGRAM LCD дисплея. Эти элементы содержат графическое представление символов: «Л» и «И»
     lcd.setCursor(5,0);                                                                      //  Устанавливаем курсор в верхний левый угол экрана (0 столбец, 0 строка)
     lcd.print(F("CHICKEN"));                                                          //  Выводим надпись «PEЛE BPEMEHИ». Буквы «Л» и «И» заменяем знаком '\' и №, под которыми их графические представления были загружены в память CGRAM LCD дисплея.
     lcd.setCursor(1,1);                                                                      //  Устанавливаем курсор в 5 столбец 1 строки (нумерация начинается с 0)
     lcd.print(F("TERMINATOR ..."));                                                             //  Выводим надпись «iarduino.ru».
     delay(5000);                                                                             //  Ждём, что бы надпись отображаемую на дисплее успели прочитать
     lcd.clear();                                                                             //  Чистим экран,
     if(!funcTestTimer()){                                                                    //  Если значения в EEPROM не соответствуют значениям таймера (например первый запуск данного скетча), то ...
         funcSetChars(14,4,2);                                                                //  Загружаем элементы 14, 4 и 2 массива rusMem в память CGRAM LCD дисплея. Эти элементы содержат графическое представление символов: «П», «Д» и «Г»
         
         for(uint8_t i=0; i<maxTimers; i++){                                                  //  Проходим по всем возможным таймерам
         for(uint8_t j=0; j<8;  j++){                                                         //  Проходим по всем возможным параметрам каждого таймера
             funcSaveTimer(i,j,0);                                                            //  Обнуляем все возможные параметры каждого таймера
         }}  delay(1000); lcd.clear();                                                        //  Ждём, что бы надпись отображаемую на дисплее успели прочитать, а потом чистим экран
     }                                                                                        //
}                                                                                             //
                                                                                              //
void loop(){                                                                                  //
     ds.requestTemperatures();                                                                //  Чтение значений датчиков температуры
     funcButton();
     funcEncoderRead();                                                                       //  Выполняем действия в соответствии с состоянием энкодера и режимом valMode
     funcDisplayUpdate();                                                                     //  Обновляем информацию на дисплее в соответствии с режимом valMode и только если установлен флаг flgDisplayUpdate
     funcSetPWM();                                                                            //  Выводим ШИМ
}                                                                                             //
                                                                                              //
//   ВЫПОЛНЕНИЕ ДЕЙСТВИЙ ПРИ ИЗМЕНЕНИИ СОТОЯНИЯ ЭНКОДЕРА И В ЗАВИСИМОСТИ ОТ ТЕКУЩЕГО РЕЖИМА   //
void funcEncoderRead(void){                                                                   //
     int i=enc.read();                                                                        //  Определяем переменную i в которую читаем состояние энкодера (может принимать 1 из 3 значений: 0, encLEFT, encRIGHT а далее к ним прибавится еще и значение encPRESS)
     int j=255;                                                                               //  Определяем переменную j для хранения № режима меню, в который требуется перейти (значение 255 означает что переход в другой режим не требуется)
     if(!digitalRead(pinEncBTN)){i=encPRESS;}                                                  //  Если нажата кнопка энкодера, то устанавливаем переменную i в значение encPRESS
     switch(valMode){                                                                         //  Далее действуем в зависимости от значения переменной valMode (которая хранит № текущего режима меню)
         case  0:               if(i==encPRESS){j= 1;}                                        //  Если установлен режим 0 "Вне меню", то ...
     /*  "00:00:00        " */  flgDisplayUpdate=1;                                           //  Устанавливаем флаг flgDisplayUpdate сигнализирующий о необходимости обновить информацию на дисплее
     /*  "00.00.0000 XX   " */                                                                //
         break;                                                                               //
         case  1:               if(i==encPRESS){j=funcReadTimer()?11:12; valTimerNum=0;}      //  Если установлен режим 1: "Меню" и нажав на энкодер, можно выбрать пункт "ТАЙМЕРЫ", то ...
     /*  "меню:           " */  if(i==encLEFT ){j= 4;}                                        //  Если энкодер поворачивается влево, то переходим в режим 3
     /*  "<   ТАЙМЕРЫ    >" */  if(i==encRIGHT){j= 2;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 2
         break;                                                                               //
         case  2:               if(i==encPRESS){j=21;}                                        //  Если установлен режим 2: "Меню" и нажав на энкодер, можно выбрать пункт "ЧАСЫ", то ...
     /*  "меню:           " */  if(i==encLEFT ){j= 1;}                                        //  Если энкодер поворачивается влево, то переходим в режим 1
     /*  "<     ЧАСЫ     >" */  if(i==encRIGHT){j= 3;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 3
         break;                                                                               //  Если установлен режим 3: "Меню" и нажав на энкодер, можно выбрать пункт "ВЫХОД", то ...
//*************************************************************************************************************************************
/**/     case  3:               if(i==encPRESS){j= 71; valArray[0]=funcReadTimer(valTimerNum,6);}
     /*  "меню:           " */  if(i==encLEFT ){j= 2;}                                        //  Если энкодер поворачивается влево, то переходим в режим 2
     /*  "< ТЕМПЕРАТУРА  >" */  if(i==encRIGHT){j= 4;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 4
/**/     break;                                                                               //
//*************************************************************************************************************************************         
         case  4:               if(i==encPRESS){j= 0; valArray[0]=valArray[1]=valArray[2]=valArray[3]=valArray[4]=0;}
     /*  "меню:           " */  if(i==encLEFT ){j= 3;}                                        //  Если энкодер поворачивается влево, то переходим в режим 2
     /*  "<    ВЫХОД     >" */  if(i==encRIGHT){j= 1;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 1
         break; 
         case 11:               if(i==encPRESS){j=51;}                                        //  Если установлен режим 11: "Меню>таймеры" и нажав на энкодер, можно выбрать один из таймеров, то ...
     /*  "меню>таймеры:   " */  if(i==encLEFT ){if(valTimerNum){valTimerNum--; lcd.clear(); flgDisplayUpdate=1;}else{j=14;}}
     /*  "< 00:00-00:00-1>" */  if(i==encRIGHT){valTimerNum++; if(valTimerNum>=maxTimers){j=13;}else if(funcReadTimer(valTimerNum)){lcd.clear(); flgDisplayUpdate=1;}else{j=12;}}
         break;                                                                               //
         case 12:               if(i==encPRESS){j=41;}                                        //  Если установлен режим 12: "Меню>таймеры" и нажав на энкодер, можно выбрать пункт "НОВЫЙ ТАЙМЕР", то ...
     /*  "меню>таймеры:   " */  if(i==encLEFT ){j=funcReadTimer()?11:14; if(funcReadTimer()){valTimerNum=funcFindTimer()-1;}}
     /*  "< НОВЫЙ ТАЙМЕР >" */  if(i==encRIGHT){j=funcReadTimer()?13:14;}                     //  Если энкодер поворачивается вправо, то переходим в режим 13 или 14 (зависит от наличия установленных таймеров)
         break;                                                                               //
         case 13:               if(i==encPRESS){j=42;}                                        //  Если установлен режим 13: "Меню>таймеры" и нажав на энкодер, можно выбрать пункт "CTEPETЬ BCE ТАЙМЕРЫ", то ...
     /*  "меню>таймеры:   " */  if(i==encLEFT ){j=funcFindTimer()<maxTimers?12:11; valTimerNum=j==11?maxTimers-1:0;}
     /*  "< CTEPETЬ BCE  >" */  if(i==encRIGHT){j=14; valTimerNum=0;}                         //  Если энкодер поворачивается вправо, то переходим в режим 14 и указываем что выбран таймер № 0
         break;                                                                               //
         case 14:               if(i==encPRESS){j= 1;}                                        //  Если установлен режим 14: "Меню>таймеры" и нажав на энкодер, можно выбрать пункт "ВЫХОД", то ...
     /*  "меню>таймеры:   " */  if(i==encLEFT ){j=funcReadTimer()?13:(funcFindTimer()<maxTimers?12:11); valTimerNum=0;}
     /*  "<    ВЫХОД     >" */  if(i==encRIGHT){j=funcReadTimer()?11:(funcFindTimer()<maxTimers?12:13); valTimerNum=0;}
         break;                                                                               //  Если установлен режим 21: "Меню>часы" и нажав на энкодер, можно выбрать пункт "ВРЕМЯ", то ...
         case 21:               if(i==encPRESS){j=31; valSubMode=0; time.gettime(); valArray[0]=time.Hours; valArray[1]=time.minutes; valArray[2]=time.seconds;}
     /*  "меню>часы:      " */  if(i==encLEFT ){j=23;}                                        //  Если энкодер поворачивается влево, то переходим в режим 23
     /*  "<    ВРЕМЯ     >" */  if(i==encRIGHT){j=22;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 22
         break;                                                                               //  Если установлен режим 22: "Меню>часы" и нажав на энкодер, можно выбрать пункт "ДАТА", то ...
         case 22:               if(i==encPRESS){j=32; valSubMode=0; time.gettime(); valArray[0]=time.day; valArray[1]=time.month; valArray[2]=time.year; valArray[3]=time.weekday;}
     /*  "меню>часы:      " */  if(i==encLEFT ){j=21;}                                        //  Если энкодер поворачивается влево, то переходим в режим 21
     /*  "<     ДАТА     >" */  if(i==encRIGHT){j=23;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 23
         break;                                                                               //
         case 23:               if(i==encPRESS){j= 2;}                                        //  Если установлен режим 23: "Меню>часы" и нажав на энкодер, можно выбрать пункт "ВЫХОД", то ...
     /*  "меню>часы:      " */  if(i==encLEFT ){j=22;}                                        //  Если энкодер поворачивается влево, то переходим в режим 22
     /*  "<    ВЫХОД     >" */  if(i==encRIGHT){j=21;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 21
         break;                                                                               //  Если установлен режим 31: "Меню>часы>вpeмя" и управляя энкодером, можно установить время, то ...
         case 31:               if(i==encPRESS){                        if(valSubMode==0){valSubMode=1;}else if(valSubMode==1){valSubMode=2;}else if(valSubMode==2){time.settime(valArray[2],valArray[1],valArray[0]); j=21;}}
     /*  "меню>часы>вpeмя:" */  if(i==encLEFT ){valArray[valSubMode]--; if(valArray[0]>23){valArray[0]=23;} if(valArray[1]>59){valArray[1]=59;} if(valArray[2]>59){valArray[2]=59;}}
     /*  "    00:00:00    " */  if(i==encRIGHT){valArray[valSubMode]++; if(valArray[0]>23){valArray[0]= 0;} if(valArray[1]>59){valArray[1]= 0;} if(valArray[2]>59){valArray[2]= 0;}}
                                flgDisplayUpdate=1;                                           //  Устанавливаем флаг flgDisplayUpdate сигнализирующий о необходимости обновить информацию на дисплее
         break;                                                                               //  Если установлен режим 32: "Меню>часы>дата" и управляя энкодером, можно установить дату, то ...
         case 32:               if(i==encPRESS){                        if(valSubMode==0){valSubMode=1;}else if(valSubMode==1){valSubMode=2;}else if(valSubMode==2){valSubMode=3;}else if(valSubMode==3){time.settime(-1,-1,-1,valArray[0],valArray[1],valArray[2],valArray[3]); j=22;}}
     /*  "MEHЮ>ЧACЫ>ДATA: " */  if(i==encLEFT ){valArray[valSubMode]--; if(valArray[0]==0){valArray[0]=31;} if(valArray[1]==0){valArray[1]=12;} if(valArray[2]>99){valArray[2]=99;} if(valArray[3]>6){valArray[3]=6;}}
     /*  " 00.00.0000 пт  " */  if(i==encRIGHT){valArray[valSubMode]++; if(valArray[0]>31){valArray[0]= 1;} if(valArray[1]>12){valArray[1]= 1;} if(valArray[2]>99){valArray[2]= 0;} if(valArray[3]>6){valArray[3]=0;}}
                                flgDisplayUpdate=1;                                           //  Устанавливаем флаг flgDisplayUpdate сигнализирующий о необходимости обновить информацию на дисплее
         break;                                                                               //
         case 41:                                                                             //  Если установлен режим 41: "HOBЫЙ TAЙMEP CO3ДAH", то данный режим сбросится в режим 61 через 2 секунды вне зависимости от состояния энкодера ...
     /*  "  HOBЫЙ TAЙMEP  " */  j=61; valTimerNum=funcFindTimer(); valSubMode=0; valArray[0]=0; valArray[1]=0; valArray[2]=0; valArray[3]=0; valArray[4]=1; funcSaveTimer(valTimerNum,0,1); funcSaveTimer(valTimerNum,1); funcSaveTimer(valTimerNum,2); funcSaveTimer(valTimerNum,3); funcSaveTimer(valTimerNum,4); funcSaveTimer(valTimerNum,5,1); funcSaveTimer(valTimerNum,6,100); funcSaveTimer(valTimerNum,7,127);
     /*  "     CO3ДAH     " */  delay(2000);                                                  //
         break;                                                                               //
         case 42:                                                                             //  Если установлен режим 42: "BCE TAЙMEPЫ УДAЛEHЫ", то данный режим сбросится в режим 12 через 2 секунды вне зависимости от состояния энкодера ...
     /*  "  BCE TAЙMEPЫ   " */  j=12; for(valArray[0]=0; valArray[0]<maxTimers; valArray[0]++){funcSaveTimer(valArray[0]);}
     /*  "    УДAЛEHЫ     " */  delay(2000);                                                  //
         break;                                                                               //
         case 43:                                                                             //  Если установлен режим 43: "TAЙMEP УДAЛEH", то данный режим сбросится в режим 11 или 12 через 2 секунды вне зависимости от состояния энкодера ...
     /*  "     TAЙMEP     " */  j=12; valArray[0]=valTimerNum; valArray[1]=funcFindTimer()-1; if(valArray[0]<valArray[1]){j=11; valTimerNum=valArray[0];}else if(valArray[1]>0){j=11; valTimerNum=valArray[0]-1;} for(int k=valArray[0]; k<valArray[1]; k++){for(uint8_t l=0; l<8; l++){funcSaveTimer(k,l,funcReadTimer(k+1,l));}} funcSaveTimer(valArray[1]);
     /*  "     УДAЛEH     " */  delay(2000);                                                  //
         break;                                                                               //  Если установлен режим 51: "Меню>тайmеры>выбранный_таймер" и нажав на энкодер, можно выбрать пункт "BPEMЯ  И KAHAЛ", то ...
         case 51:               if(i==encPRESS){j=61; valSubMode=0; valArray[0]=funcReadTimer(valTimerNum,1); valArray[1]=funcReadTimer(valTimerNum,2); valArray[2]=funcReadTimer(valTimerNum,3); valArray[3]=funcReadTimer(valTimerNum,4); valArray[4]=funcReadTimer(valTimerNum,5);}
     /*  "m>тайmеры>00:00." */  if(i==encLEFT ){j=55;}                                        //  Если энкодер поворачивается влево, то переходим в режим 55
     /*  "<BPEMЯ И KAHAЛ>" */  if(i==encRIGHT){j=52;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 52
         break;                                                                               //  Если установлен режим 52: "Меню>тайmеры>выбранный_таймер" и нажав на энкодер, можно выбрать пункт "ПOBTOPЫ", то ...
         case 52:               if(i==encPRESS){j=62; valSubMode=0; valArray[0]=bitRead(funcReadTimer(valTimerNum,7),6); valArray[1]=bitRead(funcReadTimer(valTimerNum,7),5); valArray[2]=bitRead(funcReadTimer(valTimerNum,7),4); valArray[3]=bitRead(funcReadTimer(valTimerNum,7),3); valArray[4]=bitRead(funcReadTimer(valTimerNum,7),2); valArray[5]=bitRead(funcReadTimer(valTimerNum,7),1); valArray[6]=bitRead(funcReadTimer(valTimerNum,7),0);}
     /*  "m>тайmеры>00:00." */  if(i==encLEFT ){j=51;}                                        //  Если энкодер поворачивается влево, то переходим в режим 51
     /*  "<   ПOBTOPЫ    >" */  if(i==encRIGHT){j=53;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 53
         break;                                                                               //  Если установлен режим 53: "Меню>тайmеры>выбранный_таймер" и нажав на энкодер, можно выбрать пункт "УPOBEHЬ CИГНАЛА", то ...
         case 53:               if(i==encPRESS){j=63; valArray[0]=funcReadTimer(valTimerNum,6);}
     /*  "m>тайmеры>00:00." */  if(i==encLEFT ){j=52;}                                        //  Если энкодер поворачивается влево, то переходим в режим 52
     /*  "< УPOBEHb CИГН.>" */  if(i==encRIGHT){j=54;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 54
         break;                                                                               //
         case 54:               if(i==encPRESS){j=43;}                                        //  Если установлен режим 54: "Меню>тайmеры>выбранный_таймер" и нажав на энкодер, можно выбрать пункт "CTEPETЬ TAЙMEP", то ...
     /*  "m>тайmеры>00:00." */  if(i==encLEFT ){j=53;}                                        //  Если энкодер поворачивается влево, то переходим в режим 53
     /*  "<CTEPETЬ TAЙMEP>" */  if(i==encRIGHT){j=55;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 55
         break;                                                                               //
         case 55:               if(i==encPRESS){j=11;}                                        //  Если установлен режим 55: "Меню>тайmеры>выбранный_таймер" и нажав на энкодер, можно выбрать пункт "ВЫХОД", то ...
     /*  "m>тайmеры>00:00." */  if(i==encLEFT ){j=54;}                                        //  Если энкодер поворачивается влево, то переходим в режим 54
     /*  "<    ВЫХОД     >" */  if(i==encRIGHT){j=51;}                                        //  Если энкодер поворачивается вправо, то переходим в режим 51
         break;                                                                               //  Если установлен режим 61: "Меню>тайmеры>выбранный_таймер>время_и_канал" и управляя энкодером, можно установить время и канал таймера, то ...
         case 61:               if(i==encPRESS){if(valSubMode==0){valSubMode=1;}else if(valSubMode==1){valSubMode=2;}else if(valSubMode==2){valSubMode=3;}else if(valSubMode==3){valSubMode=4;}else if(valSubMode==4){
                                j=51; funcSaveTimer(valTimerNum,0,1); funcSaveTimer(valTimerNum,1,valArray[0]); funcSaveTimer(valTimerNum,2,valArray[1]); funcSaveTimer(valTimerNum,3,valArray[2]); funcSaveTimer(valTimerNum,4,valArray[3]); funcSaveTimer(valTimerNum,5,valArray[4]);}}
     /*  "м>таймер>вpeмя: " */  if(i==encLEFT ){valArray[valSubMode]--; if(valArray[0]>23){valArray[0]=23;} if(valArray[1]>59){valArray[1]=59;} if(valArray[2]>23){valArray[2]=23;} if(valArray[3]>59){valArray[3]=59;} if(valArray[4]==0){valArray[4]=4;}}
     /*  " 00:00-00:00 к0 " */  if(i==encRIGHT){valArray[valSubMode]++; if(valArray[0]>23){valArray[0]= 0;} if(valArray[1]>59){valArray[1]= 0;} if(valArray[2]>23){valArray[2]= 0;} if(valArray[3]>59){valArray[3]= 0;} if(valArray[4]> 4){valArray[4]=1;}}
                                flgDisplayUpdate=1;                                           //  Устанавливаем флаг flgDisplayUpdate сигнализирующий о необходимости обновить информацию на дисплее
         break;                                                                               //  Если установлен режим 62: "Меню>тайmеры>выбранный_таймер>повторы" и управляя энкодером, можно установить повторы таймера, то ...
        
         case 62:               if(i==encPRESS){flgDisplayUpdate=1; if(valSubMode==0){valSubMode=1;}else if(valSubMode==1){valSubMode=2;}else if(valSubMode==2){valSubMode=3;}else if(valSubMode==3){valSubMode=4;}else if(valSubMode==4){valSubMode=5;}else if(valSubMode==5){valSubMode=6;}else if(valSubMode==6){
                                j=52; uint8_t k=0; bitWrite(k,6,valArray[0]); bitWrite(k,5,valArray[1]); bitWrite(k,4,valArray[2]); bitWrite(k,3,valArray[3]); bitWrite(k,2,valArray[4]); bitWrite(k,1,valArray[5]); bitWrite(k,0,valArray[6]); funcSaveTimer(valTimerNum,7,k); lcd.noBlink();}}
     /*  " * * * * * * *  " */  if(i==encLEFT ){flgDisplayUpdate=1; if(valArray[valSubMode]){valArray[valSubMode]=0;}else{valArray[valSubMode]=1;}}
     /*  " ^ ^ ^ ^ ^ ^ ^  " */  if(i==encRIGHT){flgDisplayUpdate=1; if(valArray[valSubMode]){valArray[valSubMode]=0;}else{valArray[valSubMode]=1;}}
         break;                                                                               //  Если установлен режим 63: "Меню>тайmеры>выбранный_таймер>уровень_сигнала" и управляя энкодером, можно установить уровень сигнала таймера, то ...
         case 63:               if(i==encPRESS){j=53; funcSaveTimer(valTimerNum,6,valArray[0]);}
     /*  "м>тaймep>cигнaл:" */  if(i==encLEFT ){flgDisplayUpdate=1; valArray[0]-=5; if(valArray[0]>100){valArray[0]=  5;} if(valArray[0]<5){valArray[0]=5;}}
     /*  "      100%      " */  if(i==encRIGHT){flgDisplayUpdate=1; valArray[0]+=5; if(valArray[0]>100){valArray[0]=100;}}
         break;                                                                               //

//*************************************************************************************************************************************
         case 71:               if(i==encPRESS){j=3; funcSaveTimer(valTimerNum,6,valArray[0]);}
     /*  "м>тaймep>темпер:" */  if(i==encLEFT ){flgDisplayUpdate=1; valArray[0]-=1; if(valArray[0]>40){valArray[0]=  40;} if(valArray[0]<0){valArray[0]=0;}}
     /*  "      40        " */  if(i==encRIGHT){flgDisplayUpdate=1; valArray[0]+=1; if(valArray[0]>40){valArray[0]=40;}}
         break;   
//*************************************************************************************************************************************     
     }                                                                                        //
     if(j<255){lcd.clear(); flgDisplayUpdate=1; valMode=j;}                                   //  Если требуется сменить режим, то чистим экран, устанавливаем флаг необходимости обновления экрана flgDisplayUpdate и устанавливаем новый режим valMode
     if(i==encPRESS){while(!digitalRead(pinEncBTN)){delay(50);}}                              //  Если была нажата кнопка энкодера, то ждём пока она не будет отпущена (с задержками по 50мс для подавления дребезга)
}                                                                                             //
                                                                                              //
//   ОБНОВЛЕНИЕ ИНФОРМАЦИИ НА ДИСПЛЕЕ                                                         //
void funcDisplayUpdate(){                                                                     //
     if(flgDisplayUpdate){      flgDisplayUpdate=0;                                           //  Обновление дисплея происходит только если был установлен флаг flgDisplayUpdate
     switch(valMode){                                                                         //  Далее действуем в зависимости от значения переменной valMode (которая хранит № текущего режима меню)
         case  0:               funcSetChars(35,36,37,5,3,0,time.weekday==4?18:14);           //  Если установлен режим 0, то загружаем символы в CGRAM лисплея в следующем порядке: 1-«1» 2-«2» 3-«3» 4-«4» 5-«г» 6-«Б» 7-«П/Ч» и выводим информацию для данного режима на дисплей ...
     /*  "00:00:00        " */  lcd.setCursor(4, 0); lcd.print(time.gettime("H:i:s"));        //  Выводим верхнюю строку
     /*  "00.00.0000г. ПН " */  //lcd.setCursor(0, 1); lcd.print(time.gettime("d."));     //
                                lcd.setCursor(0,0); lcd.print(time.weekday==1?"\7H":(time.weekday==2?"BT":(time.weekday==3?"CP":(time.weekday==4?"\7T":(time.weekday==5?"\7T":(time.weekday==6?"C\6":("BC")))))));
                                lcd.setCursor(12,0); lcd.print("    "); valArray[4]=15; if(valArray[3]){lcd.setCursor(valArray[4],0); lcd.print("\4"); valArray[4]--;} if(valArray[2]){lcd.setCursor(valArray[4],0); lcd.print("\3"); valArray[4]--;} if(valArray[1]){lcd.setCursor(valArray[4],0); lcd.print("\2"); valArray[4]--;} if(valArray[0]){lcd.setCursor(valArray[4],0); lcd.print("\1"); valArray[4]--;}
                                                                
                                if((int)ds.getTempC(sensor2)> 0){
                                lcd.setCursor(9,1);
                                lcd.print("t2 = ");
                                lcd.print((int)ds.getTempC(sensor2));}
                                else {lcd.setCursor(9,1); lcd.print("t2 =...");}

                                if((int)ds.getTempC(sensor1)> 0){
                                lcd.setCursor(0,1);
                                lcd.print("t1 = ");
                                lcd.print((int)ds.getTempC(sensor1));}
                                else {lcd.setCursor(0,1); lcd.print("t1 =...");}
         break;                                                                               //
         case  1:               funcSetChars(11,15,25,9,20);                                  //  Если установлен режим 1, то загружаем символы в CGRAM лисплея в следующем порядке: 1м 2н 3ю 4Й 5Ы и выводим информацию для данного режима на дисплей ...
     /*  "меню:           " */  lcd.setCursor(0, 0); lcd.print(F("\1e\2\3:"));                //  Выводим верхнюю строку
     /*  "<   ТАЙМЕРЫ    >" */  lcd.setCursor(0, 1); lcd.print(F("<   TA\4MEP\5    >"));      //
         break;                                                                               //
         case  2:               funcSetChars(11,15,25,18,20);                                 //  Если установлен режим 2, то загружаем символы в CGRAM лисплея в следующем порядке: 1м 2н 3ю 4Ч 5Ы и выводим информацию для данного режима на дисплей ...
     /*  "меню:           " */  lcd.setCursor(0, 0); lcd.print(F("\1e\2\3:"));                //  Выводим верхнюю строку
     /*  "<     ЧАСЫ     >" */  lcd.setCursor(0, 1); lcd.print(F("<     \4AC\5     >"));      //
         break;                                                                               //
         case  3:               funcSetChars(11,15,25,9,20);                                  //  Если установлен режим 3, то загружаем символы в CGRAM лисплея в следующем порядке: 1м 2н 3ю 4Д 5Ы и выводим информацию для данного режима на дисплей ...
     /*  "меню:           " */  lcd.setCursor(0, 0); lcd.print(F("\1e\2\3:"));                //  Выводим верхнюю строку
     /*  <  ТЕМПЕРАТУРА: >" */  lcd.setCursor(0, 1); lcd.print(F("< TEMPERATYRA >"));      //
         break;                                                                               //
         case  4:               funcSetChars(11,15,25,4,20);                                  //  Если установлен режим 4, то загружаем символы в CGRAM лисплея в следующем порядке: 1м 2н 3ю 4Д 5Ы и выводим информацию для данного режима на дисплей ...
     /*  "меню:           " */  lcd.setCursor(0, 0); lcd.print(F("\1e\2\3:"));                //  Выводим верхнюю строку
     /*  "<    ВЫХОД     >" */  lcd.setCursor(0, 1); lcd.print(F("<    B\5XO\4     >"));      //
         break;     
         case 11:               funcSetChars(15,25,17,9,21);                                  //  Если установлен режим 11, то загружаем символы в CGRAM лисплея в следующем порядке: 1н 2ю 3т 4й 5ы и выводим информацию для данного режима на дисплей ...
     /*  "меню>таймеры:   " */  lcd.setCursor(0, 0); lcd.print(F("me\1\2>\3a\4mep\5:"));      //  Выводим верхнюю строку
     /*  "< 00:00-00:00-1>" */  lcd.setCursor(0, 1); lcd.print(F("< 00:00-00:00-0>"));        //
                                lcd.setCursor(2, 1); lcd.print(funcReadTimer(valTimerNum,1)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,1));
                                lcd.setCursor(5, 1); lcd.print(funcReadTimer(valTimerNum,2)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,2));
                                lcd.setCursor(8, 1); lcd.print(funcReadTimer(valTimerNum,3)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,3));
                                lcd.setCursor(11,1); lcd.print(funcReadTimer(valTimerNum,4)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,4));
                                lcd.setCursor(14,1);                                                    lcd.print(funcReadTimer(valTimerNum,5));
         break;                                                                               //
         case 12:               funcSetChars(15,25,17,9,21,8,20);                             //  Если установлен режим 12, то загружаем символы в CGRAM лисплея в следующем порядке: 1н 2ю 3т 4й 5ы 6Й 7Ы и выводим информацию для данного режима на дисплей ...
     /*  "меню>таймеры:   " */  lcd.setCursor(0, 0); lcd.print(F("me\1\2>\3a\4mep\5:"));      //  Выводим верхнюю строку
     /*  "< НОВЫЙ ТАЙМЕР >" */  lcd.setCursor(0, 1); lcd.print(F("< HOB\7\6 TA\6MEP >"));     //
         break;                                                                               //
         case 13:               funcSetChars(15,25,17,9,21,22);                               //  Если установлен режим 13, то загружаем символы в CGRAM лисплея в следующем порядке: 1н 2ю 3т 4й 5ы 6Ь и выводим информацию для данного режима на дисплей ...
     /*  "меню>таймеры:   " */  lcd.setCursor(0, 0); lcd.print(F("me\1\2>\3a\4mep\5:"));      //  Выводим верхнюю строку
     /*  "< CTEPETь BCE  >" */  lcd.setCursor(0, 1); lcd.print(F("< CTEPET\6 BCE  >"));       //
         break;                                                                               //
         case 14:               funcSetChars(15,25,17,9,21,4,20);                             //  Если установлен режим 14, то загружаем символы в CGRAM лисплея в следующем порядке: 1н 2ю 3т 4й 5ы 6Д 7Ы и выводим информацию для данного режима на дисплей ...
     /*  "меню>таймеры:   " */  lcd.setCursor(0, 0); lcd.print(F("me\1\2>\3a\4mep\5:"));      //  Выводим верхнюю строку
     /*  "<    ВЫХОД     >" */  lcd.setCursor(0, 1); lcd.print(F("<    B\7XO\6     >"));      //
         break;                                                                               //
         case 21:               funcSetChars(11,15,25,19,21,26);                              //  Если установлен режим 21, то загружаем символы в CGRAM лисплея в следующем порядке: 1м 2н 3ю 4ч 5ы 6Я и выводим информацию для данного режима на дисплей ...
     /*  "меню>часы:      " */  lcd.setCursor(0, 0); lcd.print(F("\1e\2\3>\4ac\5:"));         //  Выводим верхнюю строку
     /*  "<    ВРЕМЯ     >" */  lcd.setCursor(0, 1); lcd.print(F("<    BPEM\6     >"));       //
         break;                                                                               //
         case 22:               funcSetChars(11,15,25,19,21,4);                               //  Если установлен режим 22, то загружаем символы в CGRAM лисплея в следующем порядке: 1м 2н 3ю 4ч 5ы 6Д и выводим информацию для данного режима на дисплей ...
     /*  "меню>часы:      " */  lcd.setCursor(0, 0); lcd.print(F("\1e\2\3>\4ac\5:"));         //  Выводим верхнюю строку
     /*  "<     ДАТА     >" */  lcd.setCursor(0, 1); lcd.print(F("<     \6ATA     >"));       //
         break;                                                                               //
         case 23:               funcSetChars(11,15,25,19,21,4,20);                            //  Если установлен режим 23, то загружаем символы в CGRAM лисплея в следующем порядке: 1м 2н 3ю 4ч 5ы 6Д 7Ы и выводим информацию для данного режима на дисплей ...
     /*  "меню>часы:      " */  lcd.setCursor(0, 0); lcd.print(F("\1e\2\3>\4ac\5:"));         //  Выводим верхнюю строку
     /*  "<    ВЫХОД     >" */  lcd.setCursor(0, 1); lcd.print(F("<    B\7XO\6     >"));      //
         break;                                                                               //
         case 31:               funcSetChars(11,15,25,19,21,1,27);                            //  Если установлен режим 31, то загружаем символы в CGRAM лисплея в следующем порядке: 1м 2н 3ю 4ч 5ы 6в 7я и выводим информацию для данного режима на дисплей ...
     /*  "меню>часы>вpeмя:" */  lcd.setCursor(0, 0); lcd.print(F("\1e\2\3>\4ac\5>\6pe\1\7:"));//  Выводим верхнюю строку
     /*  "    00:00:00    " */  lcd.setCursor(4, 1); valChar[0]=valArray[0]/10+48; valChar[1]=valArray[0]%10+48; valChar[2]=0; lcd.print((millis()%1000<500 && valSubMode==0)?"  ":valChar); lcd.print(":");
                                lcd.setCursor(7, 1); valChar[0]=valArray[1]/10+48; valChar[1]=valArray[1]%10+48; valChar[2]=0; lcd.print((millis()%1000<500 && valSubMode==1)?"  ":valChar); lcd.print(":");
                                lcd.setCursor(10,1); valChar[0]=valArray[2]/10+48; valChar[1]=valArray[2]%10+48; valChar[2]=0; lcd.print((millis()%1000<500 && valSubMode==2)?"  ":valChar);
         break;                                                                               //
         case 32:               funcSetChars(24,18,20,4,14,0);                                //  Если установлен режим 32, то загружаем символы в CGRAM лисплея в следующем порядке: 1Ю 2Ч 3Ы 4Д 5П 6Б и выводим информацию для данного режима на дисплей ...
     /*  "MEHЮ>ЧACЫ>ДATA: " */  lcd.setCursor(0, 0); lcd.print(F("MEH\1>\2AC\3>\4ATA:"));     //  Выводим верхнюю строку
     /*  " 00.00.0000 пт  " */  lcd.setCursor(1, 1); valChar[0]=valArray[0]/10+48; valChar[1]=valArray[0]%10+48; valChar[2]=0; lcd.print((millis()%1000<500 && valSubMode==0)?"  ":valChar); lcd.print(".");
                                lcd.setCursor(4, 1); valChar[0]=valArray[1]/10+48; valChar[1]=valArray[1]%10+48; valChar[2]=0; lcd.print((millis()%1000<500 && valSubMode==1)?"  ":valChar); lcd.print(".");
                                lcd.setCursor(7, 1); valChar[0]='2'; valChar[1]='0'; valChar[2]=valArray[2]/10+48; valChar[3]=valArray[2]%10+48; valChar[4]=0; lcd.print((millis()%1000<500 && valSubMode==2)?"    ":valChar);
                                lcd.setCursor(12,1); strcpy(valChar,(valArray[3]==1?"\5H":(valArray[3]==2?"BT":(valArray[3]==3?"CP":(valArray[3]==4?"\2T":(valArray[3]==5?"\5T":(valArray[3]==6?"C\6":("BC")))))))); valChar[2]=0; lcd.print((millis()%1000<500 && valSubMode==3)?"  ":valChar);
         break;                                                                               //
         case 41:               funcSetChars(20,8,4);                                         //  Если установлен режим 41, то загружаем символы в CGRAM лисплея в следующем порядке: 1Ы 2Й 3Д и выводим информацию для данного режима на дисплей ...
     /*  "  HOBЫЙ TAЙMEP  " */  lcd.setCursor(2, 0); lcd.print(F("HOB\1\2 TA\2MEP"));         //  Выводим верхнюю строку
     /*  "     CO3ДAH     " */  lcd.setCursor(5, 1); lcd.print(F("CO3\3AH"));                 //
         break;                                                                               //
         case 42:               funcSetChars(20,8,4,16,12);                                   //  Если установлен режим 42, то загружаем символы в CGRAM лисплея в следующем порядке: 1Ы 2Й 3Д 4У 5Л и выводим информацию для данного режима на дисплей ...
     /*  "  BCE TAЙMEPЫ   " */  lcd.setCursor(2, 0); lcd.print(F("BCE TA\2MEP\1"));           //  Выводим верхнюю строку
     /*  "    УДAЛEHЫ     " */  lcd.setCursor(4, 1); lcd.print(F("\4\3A\5EH\1"));             //
         break;                                                                               //
         case 43:               funcSetChars(8,16,4,12);                                      //  Если установлен режим 43, то загружаем символы в CGRAM лисплея в следующем порядке: 1Й 2У 3Д 4Л и выводим информацию для данного режима на дисплей ...
     /*  "     TAЙMEP     " */  lcd.setCursor(5, 0); lcd.print(F("TA\1MEP"));                 //  Выводим верхнюю строку
     /*  "     УДAЛEH     " */  lcd.setCursor(5, 1); lcd.print(F("\2\3A\4EH"));               //
         break;                                                                               //
         case 51:               funcSetChars(17,9,21,23,26,6,12);                             //  Если установлен режим 51, то загружаем символы в CGRAM лисплея в следующем порядке: 1т 2й 3ы 4. 5Я 6И 7Л и выводим информацию для данного режима на дисплей ...
     /*  "m>тайmеры>00:00." */  lcd.setCursor(0, 0); lcd.print(F("m>\1a\2mep\3>"));           //  Выводим верхнюю строку
     /*  "<BPEM* * KAHA*>" */   lcd.setCursor(10,0); lcd.print(funcReadTimer(valTimerNum,1)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,1)); lcd.print(":");
                                lcd.setCursor(13,0); lcd.print(funcReadTimer(valTimerNum,2)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,2)); lcd.print("\4");
                                lcd.setCursor(0, 1); lcd.print(F("<BPEM\5 \6 KAHA\7>"));     //
         break;                                                                               //
         case 52:               funcSetChars(17,9,21,23,14,20);                               //  Если установлен режим 52, то загружаем символы в CGRAM лисплея в следующем порядке: 1т 2й 3ы 4. 5П 6Ы и выводим информацию для данного режима на дисплей ...
     /*  "m>тайmеры>00:00." */  lcd.setCursor(0, 0); lcd.print(F("m>\1a\2mep\3>"));           //  Выводим верхнюю строку
     /*  "<   ПOBTOPЫ    >" */  lcd.setCursor(10,0); lcd.print(funcReadTimer(valTimerNum,1)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,1)); lcd.print(":");
                                lcd.setCursor(13,0); lcd.print(funcReadTimer(valTimerNum,2)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,2)); lcd.print("\4");
                                lcd.setCursor(0, 1); lcd.print(F("<   \5OBTOP\6    >"));      //
         break;                                                                               //
         case 53:               funcSetChars(17,9,21,23,16,6,2);                              //  Если установлен режим 53, то загружаем символы в CGRAM лисплея в следующем порядке: 1т 2й 3ы 4. 5У 6И 7Г и выводим информацию для данного режима на дисплей ...
     /*  "m>тайmеры>00:00." */  lcd.setCursor(0, 0); lcd.print(F("m>\1a\2mep\3>"));           //  Выводим верхнюю строку
     /*  "< УPOBEHb CИГН.>" */  lcd.setCursor(10,0); lcd.print(funcReadTimer(valTimerNum,1)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,1)); lcd.print(":");
                                lcd.setCursor(13,0); lcd.print(funcReadTimer(valTimerNum,2)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,2)); lcd.print("\4");
                                lcd.setCursor(0, 1); lcd.print(F("< \5POBEHb C\6\7H.>"));     //
         break;                                                                               //
         case 54:               funcSetChars(17,9,21,23,22,8);                                //  Если установлен режим 54, то загружаем символы в CGRAM лисплея в следующем порядке: 1т 2й 3ы 4. 5Ь 6Й и выводим информацию для данного режима на дисплей ...
     /*  "m>тайmеры>00:00." */  lcd.setCursor(0, 0); lcd.print(F("m>\1a\2mep\3>"));           //  Выводим верхнюю строку
     /*  "<CTEPETЬ TAЙMEP>" */  lcd.setCursor(10,0); lcd.print(funcReadTimer(valTimerNum,1)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,1)); lcd.print(":");
                                lcd.setCursor(13,0); lcd.print(funcReadTimer(valTimerNum,2)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,2)); lcd.print("\4");
                                lcd.setCursor(0, 1); lcd.print(F("<CTEPET\5 TA\6MEP>"));      //
         break;                                                                               //
         case 55:               funcSetChars(17,9,21,23,20,4);                                //  Если установлен режим 55, то загружаем символы в CGRAM лисплея в следующем порядке: 1т 2й 3ы 4. 5Ы 6Д и выводим информацию для данного режима на дисплей ...
     /*  "m>тайmеры>00:00." */  lcd.setCursor(0, 0); lcd.print(F("m>\1a\2mep\3>"));           //  Выводим верхнюю строку
     /*  "<    ВЫХОД     >" */  lcd.setCursor(10,0); lcd.print(funcReadTimer(valTimerNum,1)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,1)); lcd.print(":");
                                lcd.setCursor(13,0); lcd.print(funcReadTimer(valTimerNum,2)<10?"0":""); lcd.print(funcReadTimer(valTimerNum,2)); lcd.print("\4");
                                lcd.setCursor(0, 1); lcd.print(F("<    B\5XO\6     >"));      //
         break;                                                                               //
         case 61:               funcSetChars(11,17,9,21,1,27,10);                             //  Если установлен режим 61, то загружаем символы в CGRAM лисплея в следующем порядке: 1м 2т 3й 4ы 5в 6я 7к и выводим информацию для данного режима на дисплей ...
     /*  "м>таймер>вpeмя: " */  lcd.setCursor(0, 0); lcd.print(F("\1>\2a\3\1ep>\5pe\1\6:"));  //  Выводим верхнюю строку
     /*  " 00:00-00:00 к0 " */  lcd.setCursor(1, 1); valChar[0]=valArray[0]/10+48; valChar[1]=valArray[0]%10+48; valChar[2]=0; lcd.print((millis()%1000<500 && valSubMode==0)?"  ":valChar); lcd.print(":");
                                lcd.setCursor(4, 1); valChar[0]=valArray[1]/10+48; valChar[1]=valArray[1]%10+48; valChar[2]=0; lcd.print((millis()%1000<500 && valSubMode==1)?"  ":valChar); lcd.print("-");
                                lcd.setCursor(7, 1); valChar[0]=valArray[2]/10+48; valChar[1]=valArray[2]%10+48; valChar[2]=0; lcd.print((millis()%1000<500 && valSubMode==2)?"  ":valChar); lcd.print(":");
                                lcd.setCursor(10,1); valChar[0]=valArray[3]/10+48; valChar[1]=valArray[3]%10+48; valChar[2]=0; lcd.print((millis()%1000<500 && valSubMode==3)?"  ":valChar);
                                lcd.setCursor(13,1); valChar[0]=7;                 valChar[1]=valArray[4]   +48; valChar[2]=0; lcd.print((millis()%1000<500 && valSubMode==4)?"  ":valChar);
         break;                                                                               //
         case 62:               funcSetChars(28,29,30,31,32,33,34);                           //  Если установлен режим 62, то загружаем символы в CGRAM лисплея в следующем порядке: 1-ПН 2-ВТ 3-СР 4-ЧТ 5-ПТ 6-ВТ 7-СР и выводим информацию для данного режима на дисплей ...
     /*  " * * * * * * *  " */  lcd.setCursor(1, 0); lcd.print("\1 \2 \3 \4 \5 \6 \7");       //  Выводим верхнюю строку
     /*  " ^ ^ ^ ^ ^ ^ ^  " */  lcd.setCursor(1, 1); lcd.print(valArray[0]?"^":" "); lcd.setCursor(3, 1); lcd.print(valArray[1]?"^":" "); lcd.setCursor(5, 1); lcd.print(valArray[2]?"^":" "); lcd.setCursor(7, 1); lcd.print(valArray[3]?"^":" "); lcd.setCursor(9, 1); lcd.print(valArray[4]?"^":" "); lcd.setCursor(11,1); lcd.print(valArray[5]?"^":" "); lcd.setCursor(13,1); lcd.print(valArray[6]?"^":" ");
                                lcd.setCursor(valSubMode*2+1, 1); lcd.blink();                //
         break;                                                                               //
         case 63:               funcSetChars(11,17,9,7,3,15,13);                              //  Если установлен режим 63, то загружаем символы в CGRAM лисплея в следующем порядке: 1м 2т 3й 4и 5г 6н 7л и выводим информацию для данного режима на дисплей ...
     /*  "м>тaймep>cигнaл:" */  lcd.setCursor(0, 0); lcd.print(F("\1>\2a\3\1ep>c\4\5\6a\7:"));//  Выводим верхнюю строку
     /*  "      100%      " */  lcd.setCursor(6, 1); valChar[0]=valArray[0]/100+48; valChar[1]=valArray[0]%100/10+48; valChar[2]=valArray[0]%10+48; valChar[3]=0; lcd.print(valChar); lcd.print("%");
         break;                                                                               //
//***********************************************************************************************************
         case 71:               funcSetChars(11,17,9,7,3,15,13);                              //  Если установлен режим 63, то загружаем символы в CGRAM лисплея в следующем порядке: 1м 2т 3й 4и 5г 6н 7л и выводим информацию для данного режима на дисплей ...
     /*  "м>temperatyra:  " */  lcd.setCursor(0, 0); lcd.print(F("\1>temperatyra:"));//  Выводим верхнюю строку
     /*  "      100%      " */  lcd.setCursor(5, 1); valChar[1]=valArray[0]%100/10+48; valChar[2]=valArray[0]%10+48; valChar[3]=0; lcd.print(valChar); lcd.print(" C");
                                break;
//***********************************************************************************************************
}    }}                                                                                           //
                                                                                              //
//   ВЫВОД СИГНАЛОВ ШИМ                                                                       //
void funcSetPWM(void){                                                                        //
     static uint8_t setChanel[4] = {0,0,0,0};                                                 //  Определяем массив, каждый элемент которого соответствует уровню сигнала (от 0 до 100%) установленного на соответствующем канале
     if(valMode==0){                                                                          //  Если текущий режим равен 0 "Вне меню", то ...
         uint8_t  getChanel[4]   = {0,0,0,0};                                                 //  Определяем массив, каждый элемент которого соответствует уровню сигнала (от 0 до 100%) прочитанного из октивированного таймера для соответствующего канала
         uint32_t timeRTC        = 0;                                                         //  Определяем переменную для хранения текущего времени ввиде секунд прошедших с полуночи текущего дня (от 00:00:00)
         uint32_t timeTimerStart = 0;                                                         //  Определяем переменную для хранения времени старта таймера (для каждого таймера в теле цикла for) ввиде секунд прошедших с полуночи текущего дня (от 00:00:00)
         uint32_t timeTimerStop  = 0;                                                         //  Определяем переменную для хранения времени сброса таймера (для каждого таймера в теле цикла for) ввиде секунд прошедших с полуночи текущего дня (от 00:00:00)
         uint8_t  timeWeekday    = 0;                                                         //  Определяем переменную для хранения текущего деня недели в формате: 1-ПН, 2-ВТ, 3-СР, 4-ЧТ, 5-ПТ, 6-СБ, 7-ВС
         valArray[0]=valArray[1]=valArray[2]=valArray[3]=0;                                   //  В первые 4 элемента массива valArray будет записана 1, если на соответствующем канале будет установлен сигнал
         timeRTC = (uint32_t) time.Hours*3600+time.minutes*60+time.seconds;                   //  Получаем количество секунд прошедшее с полуночи текущего дня (от 00:00:00). Значения переменных объекта time являются актуальными, т.к. в данном режиме (valMode) в функции funcDisplayUpdate было обращение к функции time.gettime
         timeWeekday = time.weekday; if(timeWeekday==0){timeWeekday=7;}                       //  Получаем текущий день недели в формате: 1-ПН, 2-ВТ, 3-СР, 4-ЧТ, 5-ПТ, 6-СБ, 7-ВС
         for(uint8_t i=0; i<maxTimers; i++){                                                  //  Проходим по всем таймерам, ...
         if (funcReadTimer(i)){                                                               //  Если очередной таймер является установленным, то ...
         if (bitRead(funcReadTimer(i,7),7-timeWeekday)){                                      //  Если день недели повтора таймера совпал с текушим днём недели, то ...
             timeTimerStart=(uint32_t)funcReadTimer(i,1)*3600+funcReadTimer(i,2)*60;          //  Читаем время старта очередного таймера ввиде количества секунд прошедших от полуночи текущего дня (от 00:00:00)
             timeTimerStop =(uint32_t)funcReadTimer(i,3)*3600+funcReadTimer(i,4)*60;          //  Читаем время сброса очередного таймера ввиде количества секунд прошедших от полуночи текущего дня (от 00:00:00)
             if(timeTimerStart<=timeRTC && timeRTC<timeTimerStop){                            //  Если текущее время находится между временем старта и сброса таймера, то ...
                 getChanel[funcReadTimer(i,5)-1]=funcReadTimer(i,6);                          //  Читаем из таймера уровень сигнала который требуется установить на требуемом канале
                 valArray [funcReadTimer(i,5)-1]=1;                                           //  Сохраняем тот факт, что установлен сигнал на требуемом канале
             }                                                                                //
         }}}                                                                                  //
         if(setChanel[0]!=getChanel[0]){setChanel[0]=getChanel[0]; analogWrite(pinChanel_1, map(getChanel[0], 0,100, 0,255));} // выводим ШИМ на 1 канал
         if(setChanel[1]!=getChanel[1]){setChanel[1]=getChanel[1]; analogWrite(pinChanel_2, map(getChanel[1], 0,100, 0,255));} // выводим ШИМ на 2 канал
         if(setChanel[2]!=getChanel[2]){setChanel[2]=getChanel[2]; analogWrite(pinChanel_3, map(getChanel[2], 0,100, 0,255));} // выводим ШИМ на 3 канал
         if(setChanel[3]!=getChanel[3]){setChanel[3]=getChanel[3]; analogWrite(pinChanel_4, map(getChanel[3], 0,100, 0,255));} // выводим ШИМ на 4 канал
     }                                                                                        //
}                                                                                             //



void funcButton(){
int flagButton1 = 0;                // флаг состояния
int flagButton2 = 0;                // флаг состояния
int flagButton3 = 0;                // флаг состояния
int flagButton4 = 0;                // флаг состояния

//************************Режим для 1 кнопки***********************************
  if(!digitalRead(pinButton1) == HIGH && flagButton1 == 0)      
    {                                              
      digitalWrite(pinChanel_1, !digitalRead(pinChanel_1));
      flagButton1 = 1;
      
    }
  
  if(!digitalRead(pinButton1) == LOW && flagButton1 == 1)
    { 
      flagButton1 = 0;
    } 

//************************Режим для 2 кнопки***********************************
  if(!digitalRead(pinButton2) == HIGH && flagButton2 == 0)      
    {                                              
      digitalWrite(pinChanel_2, !digitalRead(pinChanel_2));
      flagButton2 = 1;
      
    }
  
  if(!digitalRead(pinButton2) == LOW && flagButton2 == 1)
    {
      flagButton2 = 0;
    }

  
  /* Сдесь глючат кнопки 3 и 4, если их не выключить и перейти в режим таймеров то включаюся реле 3 и 4*/
  
  
  
  //************************Режим для 3 кнопки*********************************** 
     if(!digitalRead(pinButton3) == HIGH)      
    { delay(50);
      //digitalWrite(pinLedButton3, HIGH);                                             
      if(ds.getTempC(sensor1) >= funcReadTimer(valTimerNum,6)){digitalWrite(pinChanel_3, HIGH);}                                 // Настройка реле включение по температуре
      if(ds.getTempC(sensor1) <= funcReadTimer(valTimerNum,6)-tgis){digitalWrite(pinChanel_3, LOW);}  
    } else {  
        digitalWrite(pinChanel_3, HIGH);
        //digitalWrite(pinLedButton3, LOW);
    }
//************************Режим для 4 кнопки***********************************
     if(!digitalRead(pinButton4) == HIGH)      
    { delay(50); 
      //digitalWrite(pinLedButton4, HIGH);                                            
      if(ds.getTempC(sensor2) >= funcReadTimer(valTimerNum,6)){digitalWrite(pinChanel_4, HIGH);}                                 // Настройка реле включение по температуре
      if(ds.getTempC(sensor2) <= funcReadTimer(valTimerNum,6)-tgis){digitalWrite(pinChanel_4, LOW);}  
    } else {  
        digitalWrite(pinChanel_4, HIGH);
        //digitalWrite(pinLedButton4, LOW);
    }       
}   


//   ЗАПИСЬ ДО 7 СИМВОЛОВ В CGRAM ДИСПЛЕЯ                                                     //
void funcSetChars(uint8_t i1, uint8_t i2, uint8_t i3, uint8_t i4, uint8_t i5, uint8_t i6, uint8_t i7){ byte i[8];
     if(i1<255){memcpy_P(i, rusMem[i1], 8); lcd.createChar(1, i);}                            //  Записываем символ i (взятый из элемента № i1 массива rusMem хранящегося в PROGMEM Arduino) в CGRAM дисплея под номером 1
     if(i2<255){memcpy_P(i, rusMem[i2], 8); lcd.createChar(2, i);}                            //  Записываем символ i (взятый из элемента № i2 массива rusMem хранящегося в PROGMEM Arduino) в CGRAM дисплея под номером 2
     if(i3<255){memcpy_P(i, rusMem[i3], 8); lcd.createChar(3, i);}                            //  Записываем символ i (взятый из элемента № i3 массива rusMem хранящегося в PROGMEM Arduino) в CGRAM дисплея под номером 3
     if(i4<255){memcpy_P(i, rusMem[i4], 8); lcd.createChar(4, i);}                            //  Записываем символ i (взятый из элемента № i4 массива rusMem хранящегося в PROGMEM Arduino) в CGRAM дисплея под номером 4
     if(i5<255){memcpy_P(i, rusMem[i5], 8); lcd.createChar(5, i);}                            //  Записываем символ i (взятый из элемента № i5 массива rusMem хранящегося в PROGMEM Arduino) в CGRAM дисплея под номером 5
     if(i6<255){memcpy_P(i, rusMem[i6], 8); lcd.createChar(6, i);}                            //  Записываем символ i (взятый из элемента № i6 массива rusMem хранящегося в PROGMEM Arduino) в CGRAM дисплея под номером 6
     if(i7<255){memcpy_P(i, rusMem[i7], 8); lcd.createChar(7, i);}                            //  Записываем символ i (взятый из элемента № i7 массива rusMem хранящегося в PROGMEM Arduino) в CGRAM дисплея под номером 7
}                                                                                             //
//   ЧТЕНИЕ ОДНОГО ИЗ ПАРАМЕТРОВ ТАЙМЕРА                                                      //
uint8_t funcReadTimer(uint8_t i, uint8_t j){return EEPROM[i*8+j];}                            //
//   ПОИСК НОВОГО ТАЙМЕРА                                                                     //
uint8_t funcFindTimer(void){uint8_t i=0; while(funcReadTimer(i)){i++; if(i>=maxTimers){break;}} return i;}
//   СОХРАНЕНИЕ ОДНОГО ИЗ ПАРАМЕТРОВ ТАЙМЕРА                                                  //
void funcSaveTimer(uint8_t i, uint8_t j, uint8_t k){EEPROM[i*8+j]=k;}                         //
//   ПРОВЕРКА ДАННЫХ ТАЙМЕРОВ В EEPROM                                                        //
bool funcTestTimer(void){for(uint8_t i=0; i<maxTimers; i++){if(funcReadTimer(i,0)>1){return false;} if(funcReadTimer(i,1)>23){return false;} if(funcReadTimer(i,2)>59){return false;} if(funcReadTimer(i,3)>23){return false;} if(funcReadTimer(i,4)>59){return false;}} return true;}

//   ПРОВЕРКА УСЛОВИЯ ДЛЯ ВКЛ/ОТКЛ РЕЛЕ ПО ТЕМПЕРАТУРЕ РЕЛЕ 4
void funcTemp2(){
      if(ds.getTempC(sensor2) >= funcReadTimer(valTimerNum,6)){digitalWrite(pinChanel_4, HIGH);}                                 // Настройка реле включение по температуре
      if(ds.getTempC(sensor2) <= funcReadTimer(valTimerNum,6)-tgis){digitalWrite(pinChanel_4, LOW);} 
}
//   ПРОВЕРКА УСЛОВИЯ ДЛЯ ВКЛ/ОТКЛ РЕЛЕ ПО ТЕМПЕРАТУРЕ РЕЛЕ 3
void funcTemp1(){
      if(ds.getTempC(sensor1) >= funcReadTimer(valTimerNum,6)){digitalWrite(pinChanel_3, HIGH);}                                 // Настройка реле включение по температуре
      if(ds.getTempC(sensor1) <= funcReadTimer(valTimerNum,6)-tgis){digitalWrite(pinChanel_3, LOW);} 
}
