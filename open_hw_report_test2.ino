/*
 * 프로젝트명 : 아두이노를 활용한 피아노
 * 제작자 : 김계홍 
 * 제작일 : 2020년 7월 
 * 목적 : Open HW 기말 대체 프로젝트 (Toy 프로젝트) 
 */

#include <SoftwareSerial.h>
#include <avr/wdt.h> // 아두이노 리셋 (와치독)
#include <TimerOne.h> //타임인터럽트(외부라이브러리)

/*
 * BUZZER : 사운드 소리 
 * TXPIN(송신) , RXPIN(수신) 블루투스 시리얼 통신용 
 * LED_1 ~ LED_8 LED제어 
 */

#define BUZZER 10

#define TXPIN 8 
#define RXPIN 9

#define LED_1 0 
#define LED_2 1
#define LED_3 2
#define LED_4 3
#define LED_5 4
#define LED_6 5
#define LED_7 6
#define LED_8 7


int octave = 3; //올타브 값 기본초기값 3

//음악 음계 
int music_scale[12][7] = 
{
  {33,65,131,262,523,1047,2093}, // 도 0
  {37,73,147,294,587,1175,2349}, // 레 1
  {41,82,165,330,659,1319,2637}, // 미 2
  {44,87,175,349,698,1397,2794}, // 파 3
  {49,98,196,392,784,1568,3136}, // 솔 4
  {55,110,220,440,880,1760,3520},// 라 5
  {62,123,247,494,968,1976,3951},// 시 6
  // 음계 샾 
  {35,69,139,277,554,1109,2217}, //도샾  7
  {39,78,156,311,622,1245,2489}, //레샾  8
  {46,93,185,370,740,1480,2960}, //파샾  9
  {52,104,208,415,831,1661,3322}, //솔샾 10
  {58,117,233,466,932,1865,3729} //라샾  11
};

//뽀로로 노래 음계 
int Pororo_music[] =
    {
      4,2,0,\
      1,1,4,4,\
      4,2,0,\
      1,5,4,\
      5,5,6,\
      4,4,5,\
      2,5,2,0,\
      2,1,1,\
      4,2,0,\
      1,1,4,4,\
      4,4,2,0,\
      1,1,5,4,\
      5,5,6,\
      4,4,5,\
      1,2,3,\
      4,\
      5,6,0,1,\
      0,6,0,\
      5,4,3,5,\
      4,3,4,2,\
      2,2,3,1,4,\
      3,3,4,2,5,\
      0,6,5,6,0,6,5,6,0,6,5,6,0,1,6,0,1,\
      4,2,0,\
      1,1,4,4,\
      4,3,0,\
      1,5,4,\
      5,1,6,\
      5,0,4,\
      1,2,3,4,\
      5,6,0,1,\
      0,6,0       
    };

//아기상어 노래 음계 
int baby_shark_music[] = 
{    
      0,1,3,3,3,3,3,3,3,0,1,3,3,3,3,3,3,0,1,3,3,3,3,3,3,3,3,2,2,\
      1,2,4,4,4,4,4,4,1,2,4,4,4,4,4,4,1,2,4,4,4,4,4,4,4,4,9,9,\
      1,2,4,4,4,4,4,4,1,2,4,4,4,4,4,4,1,2,4,4,4,4,4,4,4,4,9,9,\
      7,8,9,9,9,9,9,9,7,8,9,9,9,9,9,9,7,8,9,9,9,10,10,10 
};

//작은별 노래 음계 
int middle_star_music[] =
{
      0,0,4,4,5,5,4,\
      3,3,2,2,1,1,0,\
      4,4,3,3,2,2,1,\
      4,4,3,3,2,2,1,\
      0,0,4,4,5,5,4,\
      3,3,2,2,1,1,0      
};

SoftwareSerial BTSerial(TXPIN, RXPIN); //블루투스 Tx, Rx

void Pororo_music_function(); //뽀로로 뮤직 함수  
void baby_shark_music_function(); //아기상어 뮤직 함수  
void middle_star_music_function(); //작은별 뮤직 함수 
void interrupt_function();

void setup()  
{
  //Serial.begin(9600); // 초기화 
  BTSerial.begin(9600);
  wdt_disable();
 
  pinMode(BUZZER, OUTPUT); // 사운드 초기화 
  
  pinMode(LED_1,OUTPUT); //led 초기화 
  pinMode(LED_2,OUTPUT);
  pinMode(LED_3,OUTPUT);
  pinMode(LED_4,OUTPUT);
  pinMode(LED_5,OUTPUT);
  pinMode(LED_6,OUTPUT);
  pinMode(LED_7,OUTPUT);
  pinMode(LED_8,OUTPUT); 

  
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
  digitalWrite(LED_4, LOW);
  digitalWrite(LED_5, LOW);
  digitalWrite(LED_6, LOW);
  digitalWrite(LED_7, LOW);
  digitalWrite(LED_8, LOW);

  Timer1.initialize(100000); // 1초마다 실행. in microseconds
  Timer1.attachInterrupt(software_reset);
}


void loop()
{  
   if (BTSerial.available()) //블루투스에서 입력이 되었을때
  {    
    char cmd = (char)BTSerial.read();
    char a = cmd;
  
    /*
        ex)아스키코드 '0'은 10진수 48이므로 48-48 = 0
        2차원 배열 인덱스 0번을 지정할수 있음 
    */
    if( '0' <= cmd && cmd <= '6')
    {
      octave = cmd - 48;
    }
  
    switch(cmd) 
    {
      case 'a' : //입력된 값 확인 
      {
        digitalWrite(LED_1, HIGH); //LED제어 (켜짐)
        tone(BUZZER, music_scale[0][octave], 250); 
        //    부저 , 옥타브 표 y = 음계 x = 옥타브 ,  부저 지속시간
        delay(100); //딜레이
        digitalWrite(LED_1, LOW); //LED제어 (꺼짐)
        break; //switch문 종료 
      }

      case 'b' : 
      {
        digitalWrite(LED_2, HIGH);
        tone(BUZZER, music_scale[1][octave], 250);
        delay(100);
        digitalWrite(LED_2, LOW);
        break;        
      }

      case 'c':
      {
        digitalWrite(LED_3, HIGH);
        tone(BUZZER, music_scale[2][octave], 250);
        delay(100);
        digitalWrite(LED_3, LOW);
        break;
      }

      case 'd':
      {
        digitalWrite(LED_4, HIGH);
        tone(BUZZER, music_scale[3][octave], 250);
        delay(100);
        digitalWrite(LED_4, LOW);
        break;
      }

      case 'e':
      {
        digitalWrite(LED_5, HIGH);
        tone(BUZZER, music_scale[4][octave], 250);
        delay(100);
        digitalWrite(LED_5, LOW);
        break;
      }

      case 'f':
      {
        digitalWrite(LED_6, HIGH);
        tone(BUZZER, music_scale[5][octave], 250);
        delay(100);
        digitalWrite(LED_6, LOW);
        break;
      }

      case 'g':
      {
        digitalWrite(LED_7, HIGH);
        tone(BUZZER, music_scale[6][octave], 250);
        delay(100);
        digitalWrite(LED_7, LOW);
        break;
      }

      case 'h':
      {
        digitalWrite(LED_8, HIGH);
        tone(BUZZER, music_scale[0][octave+1], 250);
        delay(100);
        digitalWrite(LED_8, LOW);
        break;
      }

      case 'i':
      {
        tone(BUZZER, music_scale[7][octave], 250);
        break;
      }

      case 'j':
      {
        tone(BUZZER, music_scale[8][octave], 250);
        break;
      }

      case 'k':
      {
        tone(BUZZER, music_scale[9][octave], 250);
        break;
      }

      case 'l':
      {
        tone(BUZZER, music_scale[10][octave], 250);
        break;
      }

      case 'm':
      {
        tone(BUZZER, music_scale[11][octave], 250);
        break;
      }

      case 'n': //뽀로로 노래 함수호출
      {
        pororo_music_function();
        break;
      }

      case 'o': //작은별 노래 함수호출
      {
        middle_star_music_function();
        break;
      }
      
      
      case 'p': //아기상어 노래 함수호출
      {
        baby_shark_music_function();
        break;
      }      
    }
    
  }
  
}


void pororo_music_function()
{   
    int count = 0;
    //솔미솔 (노는게)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);      
    digitalWrite(Pororo_music[count], HIGH);      
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //레레솔솔 (제일좋아)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(900);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //솔미도 (친구들)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //레파솔 (모여라)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(600);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //라라시 (언제나)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(600);
    digitalWrite(Pororo_music[count], LOW);
    count++;    

    //솔솔라 (즐거워)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(600);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //미라미도 (개구쟁이)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //미레레 (뽀로로)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(900);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //솔미도 (눈덮인)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], LOW); 
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //레레솔솔 (숲속마을)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave-1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave-1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(900);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //솔솔미도 (꼬마팽귄)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(100);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    

    //레레라솔 (나가신다)
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(900);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //라라시
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(500);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    
    //솔솔라
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(500);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    //레미파솔라시도레도시도
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave+1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave+1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave+1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(BUZZER, music_scale[Pororo_music[count]][octave+1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
}


void baby_shark_music_function()
{
    int count = 0;
    
      
      //아기상어
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(500);
      digitalWrite(baby_shark_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(400);      
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);
      delay(200);     
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      //뚜루루뚜루 7번 
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      //귀여운 
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(400);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      //뚜루루루루 6번 
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);     
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      //바닷속 
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(400);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      //뚜루루뚜룻 7번
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      //상어 

      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW); 
      count++;;    

      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(800);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
            
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW); 
      count++;;
           
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(400);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
            
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);      
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(400);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(LED_4, HIGH);      
      delay(300);
      digitalWrite(LED_4, LOW);
      count++;;
      
      tone(BUZZER, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(LED_4, HIGH);      
      delay(300);
      digitalWrite(LED_4, LOW);
      count++;;
     
}

void middle_star_music_function()
{
      int count = 0;      
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(600);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(600);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(600);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);      
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(600);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(600);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(BUZZER, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(600);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
}

void software_reset()
{
    if (BTSerial.available()) //블루투스에서 입력이 되었을때
    {    
      char cmd = (char)BTSerial.read();

      if(cmd == 's')
      {        
        delay(1000);
        wdt_enable(WDTO_1S);        
      }
    }
}
