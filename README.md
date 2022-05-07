# confu_vr
VR system based on Confu HDMI to MIPI board with two OLED displays
VR шлем на основе платы Confu HDMI to MIPI.
![image](https://github.com/fa1ke5/confu_vr/blob/main/doc/board_and_oleds.jpg)


Плата содержит в себе конвертер HDMI to MIPI DSI Toshiba TC358870 и управляющий мироконтроллер STM32F103. На плате был обнаружен вывод разьема SWD, что позволило без труда считать оригинальную прошивку. В составе прошивки был обнаружен стандартный DFU загрузчик STM располагающийся по адресам 0х0000000 0х0003000, начиная со смещения 0х0003000 располагается основная прошивка микроконтрорллера. Ближняя к разьему DSI кнопка платы отвечает за передачу управления встроенному загрузчику. Необходимо зажать кнопку и подключить плату через USB к компьютеру. Плата будет обнаружена компьютером как STM Device in DFU mode. Используя утилиту DfuSe Demo от STM можно сделать резервную копию прошивки в формате .DFU и произвести загрузку новой прошивки. Интерфейс сохранения копии прошивки выделен красным на рисунке.
![image](https://github.com/fa1ke5/confu_vr/blob/main/doc/DfuSeDemo.jpg)
Загрузчик по старой китайской традиции собран абы как и имеет два интерфеса DFU, один для внутренней flash а второй (не работающий) для SPI flash, что вызывает неадекватную работу ряда сторонних утилит позволяющих работать с DFU. Для быстрой прошивки кода в плату прямо из VS Code пришлось подправить немного проект https://github.com/IntergatedCircuits/LibUsbDfu, скомпилированная утилита включена в проект. 
Для сборки проекта используется VS Code.
Быстрая прошивка кода доступна через меню "Терминал->Запуск задачи->DFU: Build and load from DFU" VS Code.
По неизвестной мне причине, разработчки платы не использовали линии аппаратного I2C процессора STM32. Что бы не менять дизайн платы пришлось использовать программный I2C в проекте.
Точки подключения I2C на плате показаны на рисунке ниже
![image](https://github.com/fa1ke5/confu_vr/blob/main/doc/connectors.jpg)
Что бы не отломить резисторы, я подпаял тонкие проводки к площадкам резисторов подтяжки и вывел I2C используя дорожки от неиспользуемых в схеме микросхем.
К шине I2C подключен датчик трекера BNO055 и OLED дисплей 128x32 на базе SSD1306. Дисплей отображает информацию регистра состояния Toshiba TC358870, загруженный EDID, прочую информацию
Дисплей можно не устанавливать, наличие дисплея код определит самостоятельно в процессе инициализации.
Датчик трекера на базе GY-BNO055 9DOF.
В качестве основы были использованы очки Fit VR, отладочный макет выглядит сегодня так
![image](https://github.com/fa1ke5/confu_vr/blob/main/doc/1651924000375.jpg)
