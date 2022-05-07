# confu_vr
VR system based on Confu HDMI to MIPI board with two OLED displays
VR шлем на основе платы Confu HDMI to MIPI.
![image](https://github.com/fa1ke5/confu_vr/blob/main/doc/board_and_oleds.jpg)


Плата содержит в себе конвертер HDMI to MIPI DSI Toshiba TC358870 и управляющий мироконтроллер STM32F103, дисплеи H381DLN01.2. На плате был обнаружен вывод разьема SWD, что позволило без труда считать оригинальную прошивку. В составе прошивки был обнаружен стандартный DFU загрузчик STM располагающийся по адресам 0х0000000 0х0003000, начиная со смещения 0х0003000 располагается основная прошивка микроконтрорллера. Ближняя к разьему DSI кнопка платы отвечает за передачу управления встроенному загрузчику. Необходимо зажать кнопку и подключить плату через USB к компьютеру. Плата будет обнаружена компьютером как STM Device in DFU mode. Используя утилиту DfuSe Demo от STM можно сделать резервную копию прошивки в формате .DFU и произвести загрузку новой прошивки. Интерфейс сохранения копии прошивки выделен красным на рисунке.
![image](https://github.com/fa1ke5/confu_vr/blob/main/doc/DfuSeDemo.jpg)

Загрузчик по старой китайской традиции собран абы как и имеет два интерфеса DFU, один для внутренней flash а второй (не работающий) для SPI flash, что вызывает неадекватную работу ряда сторонних утилит позволяющих работать с DFU. Для быстрой прошивки кода в плату прямо из VS Code пришлось подправить немного проект https://github.com/IntergatedCircuits/LibUsbDfu, скомпилированная утилита включена в проект. 
Для сборки проекта используется VS Code.
Быстрая прошивка кода доступна через меню "Терминал->Запуск задачи->DFU: Build and load from DFU" VS Code".
По неизвестной мне причине, разработчки платы не использовали линии аппаратного I2C процессора STM32. Что бы не менять дизайн платы пришлось использовать программный I2C в проекте.
Точки подключения I2C на плате показаны на рисунке ниже
![image](https://github.com/fa1ke5/confu_vr/blob/main/doc/connectors.jpg)
Что бы не отломить резисторы, я подпаял тонкие проводки к площадкам резисторов подтяжки и вывел I2C используя дорожки от неиспользуемых в схеме микросхем.
К шине I2C подключен датчик трекера BNO055 и OLED дисплей 128x32 на базе SSD1306. Дисплей отображает информацию регистра состояния Toshiba TC358870, загруженный EDID, прочую информацию
Дисплей можно не устанавливать, наличие дисплея код определит самостоятельно в процессе инициализации.
Датчик трекера на базе GY-BNO055 9DOF.
В качестве основы были использованы очки Fit VR, отладочный макет выглядит сегодня так
![image](https://github.com/fa1ke5/confu_vr/blob/main/doc/1651924000375.jpg)

Порядок действий
1. Сохранить резервную копию прошивки платы используя DfuSe Demo или через STlink SWD 
2. Перевести плату в режим DFU (зажав нижнюю кнопку подключить USB)
3. Открыть проект в VS Code, произвести сборку и прошивку проекта "Терминал->Запуск задачи->DFU: Build and load from DFU" VS Code".
4. Скачать и установить комплект программного обеспечения OSVR http://assets.razerzone.com/eeimages/osvr/HDK_Windows_Installer_Beta_0.8.0.0.exe
5. Скачать файлы конфигурации https://github.com/fa1ke5/confu_vr/blob/main/doc/srv_config.rar и распаковать содержимое в каталог C:\Program Files\HDK-Software-Suite\OSVR-Core\bin
6. В утилите HDK Tray Application выбрать Options->Custom, указать файл osvr_config_HDK_2_extended_mode_no_cam_my.json
7. Запустить сервер, установить и запустить SteamVR. Должен быть обнаружен VR шлем с трекером и запустится процедура настрoйки комнаты.

!!!!Важно

В текущих файлах конфигурации настройка дисторции соответствует оригинальному HDK OSVR
находится она в файле OSVR_HDK_2_0_my.json
раздел:
    "distortion": {
      "type": "mono_point_samples",
      "mono_point_samples_built_in": "OSVR_HDK_20_V1"
    },
Процедуру настройки выложу чуть позже......

Микропрограмма контроллера имеет встроенный USB Serial поддерживающий отладку.
Краткий список команд доступен по команде h

Commands list:

H - help
S - status register
D - Display status register on OLED
I - get chip ID
F - find I2C devices


