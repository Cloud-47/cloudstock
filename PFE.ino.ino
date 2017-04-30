//make a function to store keyvalue and KeyPressed
#include <LiquidCrystal.h>
#include <string.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

char last='N';

byte step=0;
bool can_go=true;

char input[16];
int nb_input=0;

byte site_number;
byte powertype;
byte ACDC;
byte Antonome;
byte Type_Of_Panel;
byte Type_Of_Batery;
byte Calculat;

char clean_row[]="                ";

/**
 *
 */
int print_step(){
  // clear
  lcd.clear();
  lcd.setCursor(0,0);
  // print step output
  switch(step){
      case 1: lcd.print("Enter the SITE");break;
      case 2: lcd.print("Enter the powertype");break;
      case 3: lcd.print("AC or DC");break;
      case 4: lcd.print("Antonome");break;
      case 5: lcd.print("Type Of Panel");break;
      case 6: lcd.print("Type Of Batery");break;
      case 7: lcd.print("Calulat");break;
    }
  lcd.setCursor(0,1);
  nb_input=0;
  for (int i=0;i<16;i++)input[i]=0;
}
/**
 *
 */
int next_step(){
  if(!can_go)return 1;
  if(step==7)return 2;
  // save input value
  switch(step){
      case 1: site_number=atoi(input);break;
      case 2: powertype=atoi(input);break;
      case 3: ACDC=atoi(input);break;
      case 4: Antonome=atoi(input);break;
      case 5: Type_Of_Panel=atoi(input);break;
      case 6: Type_Of_Batery=atoi(input);break;
   }
  // go to the next step
  step++;
  print_step();
  can_go=false;
}
/**
 *
 */
int previous_step(){
    step--;
    if(!step)step=1;
    print_step();
    can_go=false;
}
/**
 *
 */
int correct_step(){
  lcd.setCursor(0,1);
  lcd.print(clean_row);
  lcd.setCursor(0,1);
  can_go=false;
}
/**
 *
 */
int init_step(){
  step=0;
  lcd.clear();
  lcd.setCursor(0,0);
  for (int i=0;i<2;i++){
     input[i]=0;
  }
  nb_input=0;
  can_go=true;
  next_step();
}
/**
 *
 */
int step1_site(char kp){
  if(nb_input>14)return 1;
  if(kp>='0' && kp<='9'){
    lcd.print(kp);
    input[nb_input++]=kp;
    can_go=true;
  }
}
/**
 *
 */
int step2_powertype(char kp){
  if(nb_input>14)return 1;
  lcd.print(kp);
  input[nb_input++]=kp;
   can_go=true;
}
/**
 *
 */
int step3_ACDC(char kp){
  if(nb_input>15)return 1;
  lcd.print(kp);
  input[nb_input++]=kp;
  can_go=true;
}
/**
 *
 */
int step4_Antonome(char kp){
  if(nb_input>15)return 1;
  lcd.print(kp);
  input[nb_input++]=kp;
  can_go=true;
}
/**
 *
 */
int step5_Type_Of_Panel(char kp){
  if(nb_input>15)return 1;
  lcd.print(kp);
  input[nb_input++]=kp;
  can_go=true;
}
/**
 *
 */
int step6_Type_Of_Batery(char kp){
  if(nb_input>15)return 1;
  lcd.print(kp);
  input[nb_input++]=kp;
  can_go=true;
}
/**
 *
 */
int step7_Calculate(){

}
/**
 *
 */
char ReadKB(){
  int KV0,KV1,KV2,KV3;
  KV0 = analogRead(A0);
  if(KV0>=700 && KV0<=1000 ){return 'A';}
  if(KV0>=400 && KV0<=700 ){return 'B';}
  if(KV0>=100 && KV0<=250 ){return 'C';}
  if(KV0>=10 && KV0<=100){return 'D';}
  KV1 = analogRead(A1);
  if(KV1>=700 && KV1<=1000 ){return '3';}
  if(KV1>=400 && KV1<=700 ){return '6';}
  if(KV1>=100 && KV1<=250 ){return '9';}
  if(KV1>=10 && KV1<=100 ){return '#';}
  KV2 = analogRead(A2);
  if(KV2>=700 && KV2<=1000 ){return '2';}
  if(KV2>=400 && KV2<=700 ){return '5';}
  if(KV2>=100 && KV2<=250 ){return '8';}
  if(KV2>=10 && KV2<=100 ){return '0';}
  KV3 = analogRead(A3);
  if(KV3>=700 && KV3<=1000 ){return '1';}
  if(KV3>=400 && KV3<=700 ){return '4';}
  if(KV3>=100 && KV3<=250 ){return '7';}
  if(KV3>=10 && KV3<=100 ){return '*';}
 //if(KV0==0 && KV1==0 && KV2==0 && KV3==0){return 'N';}
  return('N');
}
/**
 *
 */
void setup(){
  lcd.begin(16, 2);
  lcd.cursor();
  lcd.print("Welcome!");
  delay(1000);
  Serial.begin(9600);
  //lcd.clear();
  lcd.setCursor(0,0);
  init_step();
}
/**
 *
 */
void loop(){
  char KP;
  KP = ReadKB();
  if(KP!='N' && KP!=last){
    if(KP=='#'){
      Serial.print("Next Step");
      next_step();
    }
    else
    if(KP=='*'){
      Serial.print("Privious Step");
      previous_step();
    }
    else
    if(KP=='C'){
      Serial.print("Correct Step");
      correct_step();
    }
    else
    switch(step){
      case 1: step1_site(KP);break;
      case 2: step2_powertype(KP);break;
      case 3: step3_ACDC(KP);break;
      case 4: step4_Antonome(KP);break;
      case 5: step5_Type_Of_Panel(KP);break;
      case 6: step6_Type_Of_Batery(KP);break;
      case 7: step7_Calculate();break;
    }
  }
  last=KP;
  delay(100);
}

