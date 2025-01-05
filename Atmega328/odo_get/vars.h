#define CLOCK 2  // Тактирующий сигнал с приборки
#define LOAD 3  // Сигнал на отправку данных с приборки на дисплей
#define DATA 4  // Данные
#define LIGHT 5
#define ENG_LOAD PB0
#define ENG_RPM PB1
#define CAR_SPEED PB2


volatile uint32_t buf[4];
/*volatile uint32_t filter[5];
volatile uint16_t filterDayOdo[5];
volatile byte filterOdoCount[5];
volatile byte filterCount;*/

volatile uint32_t odo = 0;  // Пробег основной
volatile uint16_t dayOdo = 0; //Пробег суточный

uint16_t FLoad = 0;
uint16_t FRpm = 0;
uint16_t FSpeed = 0;

volatile unsigned long lastTime[3] = {0, 0, 0};  // Время последнего фронта для каждого пина
volatile unsigned long period[3] = {0, 0, 0};   // Период сигнала для каждого пина
volatile bool lastState[3] = {LOW, LOW, LOW};   // Предыдущее состояние для каждого пина
