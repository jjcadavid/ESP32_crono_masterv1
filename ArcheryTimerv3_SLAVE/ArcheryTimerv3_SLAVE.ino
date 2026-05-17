#include <WiFi.h>
#include <esp_now.h>

//const int BUZZER_PIN = 25;
int pwmChannel;

// Estructura del mensaje
typedef struct {
  uint8_t comando;  // 1 = ON, 0 = OFF
} mensaje_t;

mensaje_t mensajeR;
//********

int digit[] = { 23, 22, 21, 19 };
int segments[] = { 18, 17, 2, 4, 16, 5, 15 };  //a,b,c,d,e,f,g


// --- Variables ---
const int numPins = sizeof(digit) / sizeof(digit[0]);
const int numSegments = sizeof(segments) / sizeof(segments[0]);
int timesSetup[] = { 0, 160, 120, 90, 30, 10 };
int t0, t1, tf;
int cont = -1;
String mensaje;
bool play = false;
bool inicia = false;
//int timeSelect;
int section = 0;
int ts;

const int BUZZER_PIN = 12;

//*******


// Callback de recepción (API nueva)
void onReceive(const esp_now_recv_info *info,
               const uint8_t *data,
               int len) {

  // Copia los datos recibidos
  memcpy(&mensajeR, data, sizeof(mensajeR));

  // ---- SALIDA POR SERIAL ----
  Serial.print("Mensaje recibido | Comando: ");
  Serial.print(mensajeR.comando);
  Serial.print(" | Desde MAC: ");

  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", info->src_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  // ---- ACCIÓN ----
  if (mensajeR.comando == 1) {
    ledcWriteTone(pwmChannel, 2000);  // Beep
  } else {
    ledcWriteTone(pwmChannel, 0);  // Silencio
  }

  //*************
  if (mensajeR.comando == 1) {
    mensaje="1";
    _Start();
  } else if (mensajeR.comando == 2) {
    mensaje="2";
    _Start();
  } else if (mensajeR.comando == 3) {
    mensaje="3";
    _Start();
  } else if (mensajeR.comando == 4) {
    mensaje="4";
    _Start();
  } else if (mensajeR.comando == 5) {
  } else if (mensajeR.comando == 6) {
  } else if (mensajeR.comando == 7) {
  } else if (mensajeR.comando == 8) {
  } else if (mensajeR.comando == 9) {
    _Start();
  } else if (mensajeR.comando == 0) {
    mensaje="0";
    _Stop();
  } else {
    //int valor = mensajeR.comando.toInt();
  }
  //*************
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Receptora iniciando...");

  // PWM para buzzer
  //pwmChannel = ledcAttach(BUZZER_PIN, 2000, 8);

  // ESP-NOW
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  Serial.println("Receptora lista, esperando mensajes...");

  // Configurar pines digitales como salidas
  for (int i = 0; i < numPins; i++) {
    pinMode(digit[i], OUTPUT);
    digitalWrite(digit[i], LOW);
  }
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  for (int i = 0; i < numSegments; i++) {
    pinMode(segments[i], OUTPUT);
    digitalWrite(segments[i], HIGH);
  }
  t0 = millis();
}

void loop() {
  controlBuzzer();
  if (play == false && inicia == true) {
    play = true;
    t0 = millis();
    cont = timesSetup[5];
    ts = cont;

  } else if (play == true && inicia == true) {
    t1 = millis();
    tf = t1 - t0;
    if (tf > 985) {
      cont--;
      t0 = millis();
      if (cont < 0 && inicia == true) {
        inicia == false;
        if (mensaje == "1") {
          cont = timesSetup[1];
        } else if (mensaje == "2") {
          cont = timesSetup[2];
        } else if (mensaje == "3") {
          cont = timesSetup[3];
        } else if (mensaje == "4") {
          cont = timesSetup[4];
        }
        ts = cont;
        mensaje = "";
      } else if (cont < 0 && inicia == true) {
        _Stop();
      }
    }
  }
  displayNumber(cont);
}


//*************
void displayNumber(int num) {
  int centenas = int(num / 100);
  int decenas = int((num - centenas * 100) / 10);
  int unidades = num - centenas * 100 - decenas * 10;

  displayNumbers(2, unidades);  //3
  delay(5);
  if (num >= 10) displayNumbers(1, decenas);  //2
  delay(5);
  if (num >= 100) displayNumbers(0, centenas);  //1
  delay(5);
}
void displayNumbers(int d, int n) {
  switch (d) {
    case 0:
      digitalWrite(digit[0], HIGH);
      digitalWrite(digit[1], LOW);
      digitalWrite(digit[2], LOW);
      digitalWrite(digit[3], LOW);
      break;
    case 1:
      digitalWrite(digit[0], LOW);
      digitalWrite(digit[1], HIGH);
      digitalWrite(digit[2], LOW);
      digitalWrite(digit[3], LOW);
      break;
    case 2:
      digitalWrite(digit[0], LOW);
      digitalWrite(digit[1], LOW);
      digitalWrite(digit[2], HIGH);
      digitalWrite(digit[3], LOW);
      break;
    case 3:
      digitalWrite(digit[0], LOW);
      digitalWrite(digit[1], LOW);
      digitalWrite(digit[2], LOW);
      digitalWrite(digit[3], HIGH);
      break;
    default:
      digitalWrite(digit[0], LOW);
      digitalWrite(digit[1], LOW);
      digitalWrite(digit[2], LOW);
      digitalWrite(digit[3], LOW);
      break;
  }
  number(n);
}
void number(int n) {
  for (int i = 0; i < numSegments; i++) {
    digitalWrite(segments[i], HIGH);
  }
  switch (n) {
    case 0:
      digitalWrite(segments[0], LOW);
      digitalWrite(segments[1], LOW);
      digitalWrite(segments[2], LOW);
      digitalWrite(segments[3], LOW);
      digitalWrite(segments[4], LOW);
      digitalWrite(segments[5], LOW);
      digitalWrite(segments[6], HIGH);
      break;
    case 1:
      digitalWrite(segments[0], HIGH);
      digitalWrite(segments[1], LOW);
      digitalWrite(segments[2], LOW);
      digitalWrite(segments[3], HIGH);
      digitalWrite(segments[4], HIGH);
      digitalWrite(segments[5], HIGH);
      digitalWrite(segments[6], HIGH);
      break;
    case 2:
      digitalWrite(segments[0], LOW);
      digitalWrite(segments[1], LOW);
      digitalWrite(segments[2], HIGH);
      digitalWrite(segments[3], LOW);
      digitalWrite(segments[4], LOW);
      digitalWrite(segments[5], HIGH);
      digitalWrite(segments[6], LOW);
      break;
    case 3:
      digitalWrite(segments[0], LOW);
      digitalWrite(segments[1], LOW);
      digitalWrite(segments[2], LOW);
      digitalWrite(segments[3], LOW);
      digitalWrite(segments[4], HIGH);
      digitalWrite(segments[5], HIGH);
      digitalWrite(segments[6], LOW);
      break;
    case 4:
      digitalWrite(segments[0], HIGH);
      digitalWrite(segments[1], LOW);
      digitalWrite(segments[2], LOW);
      digitalWrite(segments[3], HIGH);
      digitalWrite(segments[4], HIGH);
      digitalWrite(segments[5], LOW);
      digitalWrite(segments[6], LOW);
      break;
    case 5:
      digitalWrite(segments[0], LOW);
      digitalWrite(segments[1], HIGH);
      digitalWrite(segments[2], LOW);
      digitalWrite(segments[3], LOW);
      digitalWrite(segments[4], HIGH);
      digitalWrite(segments[5], LOW);
      digitalWrite(segments[6], LOW);
      break;
    case 6:
      digitalWrite(segments[0], LOW);
      digitalWrite(segments[1], HIGH);
      digitalWrite(segments[2], LOW);
      digitalWrite(segments[3], LOW);
      digitalWrite(segments[4], LOW);
      digitalWrite(segments[5], LOW);
      digitalWrite(segments[6], LOW);
      break;
    case 7:
      digitalWrite(segments[0], LOW);
      digitalWrite(segments[1], LOW);
      digitalWrite(segments[2], LOW);
      digitalWrite(segments[3], HIGH);
      digitalWrite(segments[4], HIGH);
      digitalWrite(segments[5], HIGH);
      digitalWrite(segments[6], HIGH);
      break;
    case 8:
      digitalWrite(segments[0], LOW);
      digitalWrite(segments[1], LOW);
      digitalWrite(segments[2], LOW);
      digitalWrite(segments[3], LOW);
      digitalWrite(segments[4], LOW);
      digitalWrite(segments[5], LOW);
      digitalWrite(segments[6], LOW);
      break;
    case 9:
      digitalWrite(segments[0], LOW);
      digitalWrite(segments[1], LOW);
      digitalWrite(segments[2], LOW);
      digitalWrite(segments[3], LOW);
      digitalWrite(segments[4], HIGH);
      digitalWrite(segments[5], LOW);
      digitalWrite(segments[6], LOW);
      break;
  }
}

void _Start() {
  inicia = true;
  play = false;
}
void _Stop() {
  cont = -1;
  inicia = false;
  play = false;
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    delay(1000);
  }
}


void controlBuzzer() {
  Serial.println(cont);
  Serial.println(ts);
  if (ts == 10 && cont == 10) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (ts == 10 && cont == 9) {
    digitalWrite(BUZZER_PIN, LOW);
  } else if (ts == 10 && cont == 8) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (ts == 10 && cont == 7) {
    digitalWrite(BUZZER_PIN, LOW);
  }
  if (ts > 10 && cont == timesSetup[1]) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (ts > 10 && cont == timesSetup[1] - 1) {
    digitalWrite(BUZZER_PIN, LOW);
  }
  if (ts > 10 && cont == timesSetup[2]) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (ts > 10 && cont == timesSetup[2] - 1) {
    digitalWrite(BUZZER_PIN, LOW);
  }
  if (ts > 10 && cont == timesSetup[3]) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (ts > 10 && cont == timesSetup[3] - 1) {
    digitalWrite(BUZZER_PIN, LOW);
  }
  if (ts > 10 && cont == timesSetup[4]) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (ts > 10 && cont == timesSetup[4] - 1) {
    digitalWrite(BUZZER_PIN, LOW);
  }

  if (cont == 0) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (cont == -1) {
    digitalWrite(BUZZER_PIN, LOW);
  } else if (cont == -2) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (cont == -3) {
    digitalWrite(BUZZER_PIN, LOW);
  } else if (cont == -4) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (cont == -5) {
    digitalWrite(BUZZER_PIN, LOW);
  }
}