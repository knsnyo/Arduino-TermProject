// 0에서 9까지 숫자 표현을 위한
// 세그먼트 a, b, c, d, e, f, g, dp의 패턴
byte patterns[] = {
  0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6
};
int digit_select_pin[] = {66, 67, 68, 69}; // 자릿수 선택 핀
// 7세그먼트 모듈 연결 핀 ‘a, b, c, d, e, f, g, dp’ 순서
int segment_pin[] = {58, 59, 60, 61, 62, 63, 64, 65};
int SEGMENT_DELAY = 5; // 숫자 표시 사이의 시간 간격

// 가변저항 숫자제어
unsigned long time = 0;

// 시간제어
unsigned long time_previouse, time_current;

// 버튼
int button = 14;

// 모터
int able = 38;
int pwm = 9;
int dir = 39;

void setup() {
  Serial.begin(9600);
  // 세그먼트
  for (int i = 0; i < 4; i++) { // 자릿수 선택 핀을 출력으로 설정
    pinMode(digit_select_pin[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++) { // 세그먼트 제어 핀을 출력으로 설정
    pinMode(segment_pin[i], OUTPUT);
  }
  
  // 스위치
  pinMode(button, INPUT);

  // 모터
  pinMode(able, OUTPUT);
  digitalWrite(able, LOW); // 제어 불가능
  pinMode(pwm, OUTPUT);
  pinMode(dir, OUTPUT);
}

// 해당 자리에 숫자 하나를 표시하는 함수
void show_digit(int pos, int number) { // (위치, 출력할 숫자)
  for (int i = 0; i < 4; i++) {
    if (i + 1 == pos) // 해당 자릿수의 선택 핀만 LOW로 설정
      digitalWrite(digit_select_pin[i], LOW);
    else // 나머지 자리는 HIGH로 설정
      digitalWrite(digit_select_pin[i], HIGH);
  }
  for (int i = 0; i < 8; i++) {
    boolean on_off = bitRead(patterns[number], 7 - i);
    digitalWrite(segment_pin[i], on_off);
  }
}

// 4자리 7세그먼트 표시 장치에 4자리 숫자를 표시하는 함수
void show_4_digit(int _number) {
  int number = _number;
  number = number % 10000; // 4자리로 제한
  int thousands = number / 1000; // 천 자리
  number = number % 1000;
  int hundreads = number / 100; // 백 자리
  number = number % 100;
  int tens = number / 10; // 십 자리
  int ones = number % 10; // 일 자리
  if (_number > 999) show_digit(1, thousands);
  delay(SEGMENT_DELAY);
  if (_number > 99) show_digit(2, hundreads);
  delay(SEGMENT_DELAY);
  if (_number > 9) show_digit(3, tens);
  delay(SEGMENT_DELAY);
  show_digit(4, ones);
  delay(SEGMENT_DELAY);
}

void count_down(int set_time){
  Serial.println("function()");
  int current_time;
  time_previouse = millis();
  while(1){
    Serial.println(time);
    time_current = millis();
    if(1000 <= time_current - time_previouse){
      time -= 1;
      time_previouse = time_current;
    }
    if(0 >= time){
      break;
    }
    show_4_digit(time);
    // 모터 뺑뺑이
    digitalWrite(able, HIGH);
    digitalWrite(dir, HIGH); // 정방향
    digitalWrite(pwm, LOW); // 최고속도
  }
  // 모터 끄기
  digitalWrite(pwm, HIGH); // 속도 0
  digitalWrite(able, LOW);
}

void loop() {
  int reading = analogRead(A0); // 가변저항 값 읽기
  time = map(reading, 0, 1023, 0, 180);
  show_4_digit(time); // 가변저항 값 표시
  
  if(digitalRead(button)){
    Serial.println("hi~!");
    // 모터 시간만큼 돌리기~
    // 시간도 줄이기~
    count_down(time);
  }
}
