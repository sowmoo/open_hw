#include <SoftwareSerial.h>
#include <avr/wdt.h> //와치독 (시간체크) 
#include <TimerOne.h> //타임인터럽트(외부라이브러리)


//전역변수 


int buzzer = 10; //사운드

int TxPin = 8; //블루투스
int RxPin = 9;



int led_1 = 0; 
int led_2 = 1;
int led_3 = 2;
int led_4 = 3;
int led_5 = 4;
int led_6 = 5;
int led_7 = 6;
int led_8 = 7;

int intrrupt = 3;

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

SoftwareSerial BTSerial(TxPin, RxPin); //블루투스 Tx, Rx

void Pororo_music_function(); //뽀로로 뮤직 함수  
void baby_shark_music_function(); //아기상어 뮤직 함수  
void middle_star_music_function(); //작은별 뮤직 함수 
void interrupt_function();

void setup()  
{
  //Serial.begin(9600); // 초기화 
  BTSerial.begin(9600);
  wdt_disable();
 
  pinMode(buzzer, OUTPUT); // 사운드 초기화 
  
  pinMode(led_1,OUTPUT); //led 초기화 
  pinMode(led_2,OUTPUT);
  pinMode(led_3,OUTPUT);
  pinMode(led_4,OUTPUT);
  pinMode(led_5,OUTPUT);
  pinMode(led_6,OUTPUT);
  pinMode(led_7,OUTPUT);
  pinMode(led_8,OUTPUT); 

  
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
  digitalWrite(led_5, LOW);
  digitalWrite(led_6, LOW);
  digitalWrite(led_7, LOW);
  digitalWrite(led_8, LOW);

  attachInterrupt(intrrupt, 0, FALLING);

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
        digitalWrite(led_1, HIGH); //LED제어 (켜짐)
        tone(buzzer, music_scale[0][octave], 250); 
        //    부저 , 옥타브 표 y = 음계 x = 옥타브 ,  부저 지속시간
        delay(100); //딜레이
        digitalWrite(led_1, LOW); //LED제어 (꺼짐)
        break; //switch문 종료 
      }

      case 'b' :
      {
        digitalWrite(led_2, HIGH);
        tone(buzzer, music_scale[1][octave], 250);
        delay(100);
        digitalWrite(led_2, LOW);
        break;        
      }

      case 'c':
      {
        digitalWrite(led_3, HIGH);
        tone(buzzer, music_scale[2][octave], 250);
        delay(100);
        digitalWrite(led_3, LOW);
        break;
      }

      case 'd':
      {
        digitalWrite(led_4, HIGH);
        tone(buzzer, music_scale[3][octave], 250);
        delay(100);
        digitalWrite(led_4, LOW);
        break;
      }

      case 'e':
      {
        digitalWrite(led_5, HIGH);
        tone(buzzer, music_scale[4][octave], 250);
        delay(100);
        digitalWrite(led_5, LOW);
        break;
      }

      case 'f':
      {
        digitalWrite(led_6, HIGH);
        tone(buzzer, music_scale[5][octave], 250);
        delay(100);
        digitalWrite(led_6, LOW);
        break;
      }

      case 'g':
      {
        digitalWrite(led_7, HIGH);
        tone(buzzer, music_scale[6][octave], 250);
        delay(100);
        digitalWrite(led_7, LOW);
        break;
      }

      case 'h':
      {
        digitalWrite(led_8, HIGH);
        tone(buzzer, music_scale[0][octave+1], 250);
        delay(100);
        digitalWrite(led_8, LOW);
        break;
      }

      case 'i':
      {
        tone(buzzer, music_scale[7][octave], 250);
        break;
      }

      case 'j':
      {
        tone(buzzer, music_scale[8][octave], 250);
        break;
      }

      case 'k':
      {
        tone(buzzer, music_scale[9][octave], 250);
        break;
      }

      case 'l':
      {
        tone(buzzer, music_scale[10][octave], 250);
        break;
      }

      case 'm':
      {
        tone(buzzer, music_scale[11][octave], 250);
        break;
      }

      case 'n':
      {
        pororo_music_function();
        break;
      }

      case 'o':
      {
        middle_star_music_function();
        break;
      }
      
      
      case 'p':
      {
        baby_shark_music_function();
        break;
      }

      case 's':
      {
        digitalWrite(2, LOW);
      }
      
    }
    
  }
  
}


void pororo_music_function()
{   
    int count = 0;
    //솔미솔 (노는게)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);      
    digitalWrite(Pororo_music[count], HIGH);      
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //레레솔솔 (제일좋아)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(900);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //솔미도 (친구들)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //레파솔 (모여라)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(600);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //라라시 (언제나)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(600);
    digitalWrite(Pororo_music[count], LOW);
    count++;    

    //솔솔라 (즐거워)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(600);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //미라미도 (개구쟁이)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //미레레 (뽀로로)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(900);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //솔미도 (눈덮인)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], LOW); 
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //레레솔솔 (숲속마을)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave-1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave-1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(900);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //솔솔미도 (꼬마팽귄)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(100);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(300);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    

    //레레라솔 (나가신다)
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(900);
    digitalWrite(Pororo_music[count], LOW);
    count++;

    //라라시
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(500);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    
    //솔솔라
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(500);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    //레미파솔라시도레도시도
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave+1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave+1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(200);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave+1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
    tone(buzzer, music_scale[Pororo_music[count]][octave+1], 200);
    digitalWrite(Pororo_music[count], HIGH);    
    delay(400);
    digitalWrite(Pororo_music[count], LOW);
    count++;
    
}


void baby_shark_music_function()
{
    int count = 0;
    
      
      //아기상어
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(500);
      digitalWrite(baby_shark_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(400);      
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);
      delay(200);     
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      //뚜루루뚜루 7번 
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      //귀여운 
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(400);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      //뚜루루루루 6번 
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);     
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      //바닷속 
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(400);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      //뚜루루뚜룻 7번
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
    
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      //상어 

      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW); 
      count++;;    

      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(800);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
            
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW); 
      count++;;
           
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(400);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
            
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);      
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(400);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;

      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(100);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(200);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(baby_shark_music[count], HIGH);      
      delay(300);
      digitalWrite(baby_shark_music[count], LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(led_4, HIGH);      
      delay(300);
      digitalWrite(led_4, LOW);
      count++;;
      
      tone(buzzer, music_scale[baby_shark_music[count]][octave], 200);
      digitalWrite(led_4, HIGH);      
      delay(300);
      digitalWrite(led_4, LOW);
      count++;;
     
}

void middle_star_music_function()
{
      int count = 0;      
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(600);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(600);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(600);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);      
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(600);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(600);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
      digitalWrite(middle_star_music[count], HIGH);
      delay(400);
      digitalWrite(middle_star_music[count], LOW);
      count++;
      
      tone(buzzer, music_scale[middle_star_music[count]][octave], 200);
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
        Serial.print("RESET \n");
        delay(1000);
        wdt_enable(WDTO_1S);
        Serial.print("NOTHING!!! \n");
      }
    }
}
