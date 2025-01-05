#define MAX_DIFF 5

int arrowAng = 0;
int arrowAngSet = 0;

byte disp = 1;

uint32_t odoKm = 0;
uint16_t odoDayKm = 0;
uint8_t spd = 0;
uint8_t spdLimit = 240;
uint8_t spdLimitFlag = 0;
uint16_t spdLimitColor = TFT_BLACK;
uint8_t imageId = 0;

String fnt = "Final-Frontier-28";

uint16_t color = 63488;

float x = 0;
float y = 0;
float a = 0;
float b = 0;
float start = 0;
float end = 0;
uint32_t cl = 0;
String inputString = ""; // Буфер для входных данных
bool stringComplete = false;

uint16_t FLoad = 0;
uint16_t FRpm = 0;
uint16_t FSpeed = 0;

const char* ssid = "BMW_DashBoard";      // Имя точки доступа
const char* password = "16891689";

bool isInitialized = false;
volatile long lastValue;
long lastValue2;
volatile bool isInitialized2 = false;
