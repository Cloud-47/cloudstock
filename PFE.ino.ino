//make a function to store keyvalue and KeyPressed
#include <LiquidCrystal.h>
#include <string.h>
//Constants

//Courant
#define AC 0
#define DC 1
//SITE
#define ALGER     1
#define BOUMERDES 2
#define OURAN     3
//Irradiation
#define Ir_Alger 100
#define Ir_BOUMERDES 120
#define Ir_ORAN 90
//Coefficient correcteur
#define K 0.55
//Volatge utilisé
#define _12V 1
#define _24V 2
#define _48V 3

//Type de panneau
#define _100watts  1
#define _200watts  2
#define _300watts  3
#define _500watts  5

//type de batterie
#define pas_batterie  0
#define batterie_100W 1
#define batterie_200W 2
#define batterie_500W 5

//Fonction d'affichage
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//dernier touche tapé
char last='N';

//Etape dans le programme
byte step=0;
bool can_go=true;

//
char input[16];
int nb_input=0;

//
byte site_number;
double Ec;//Energie consommée
byte VoltageAmount;
byte ACDC;
byte Antonome;
byte Type_Of_Panel;
byte Type_Of_Batery;

bool calculated=false;
//
char clean_row[]="                ";


//Valeur calculer par le programme
double Pch;//Puissance crete
double Vch;//tension utilisée par la charge
double Vn;// tension nominal
double Pc;//Puissance des panneaux

int Nm;//Nombre de module
int Ns;//Nombre de module en série
int Np;//nombre de module en parallel


/**
 *affichage de parapatre sur LCDon
 */
int print_step(){
  lcd.clear();
  lcd.setCursor(0,0);
  switch(step){
      case 1: lcd.print("Enter the SITE");break;
      case 2: lcd.print("Enter the Power Amount");break;
      case 3: lcd.print("Enter the Volatge Amount");break;
      case 4: lcd.print("AC or DC");break;
      case 5: lcd.print("Antonome");break;
      case 6: lcd.print("Type Of Panel");break;
      case 7: lcd.print("Type Of Batery");break;
      case 8: lcd.print("Calulat");break;
      case 9: step9_NemberOfPanel();break;
      case 10: lcd.print("on série");break;
      case 11: lcd.print("on parelel")break;
      case 12: lcd.print("cost");break;
      case 13: init_step();break;
    }
  lcd.setCursor(0,1);
  nb_input=0;
  for (int i=0;i<16;i++)input[i]=0;
}
/**
 *regulatteur des Etape re suivant et recupare les input
 */
int next_step(){
  if(calculated){
    step++;
    print_step();
  }
  if(!can_go)return 2;
  // save input value
  switch(step){
      case 1: site_number=atoi(input);break;
      case 2: Ec=atoi(input);break;
      case 3: VoltageAmount=atoi(input);break;
      case 4: ACDC=atoi(input);break;
      case 5: Antonome=atoi(input);break;
      case 6: Type_Of_Panel=atoi(input);break;
      case 7: Type_Of_Batery=atoi(input);break;
   }
  // go to the next step
  step++;
  print_step();
  can_go=false;
}
/**
 *Revenir à l'étape précédante
 */
int previous_step(){
    step--;
    if(!step)step=1;
    print_step();
    can_go=false;
}
/**
 *Effacer l'écran
 */
int correct_step(){

  lcd.setCursor(0,1);
  lcd.print(clean_row);
  lcd.setCursor(0,1);
  can_go=false;
  nb_input=0;
  for (int i=0;i<16;i++)input[i]=0;

}
/**
 *Réinitialiser le programme
 */
int init_step(){
  lcd.setCursor(0,1);
  lcd.print(clean_row);
  lcd.setCursor(0,1);
  nb_input=0;
  for (int i=0;i<16;i++)input[i]=0;
  step=0;
  can_go=true;
  calculated=false;
  next_step();
}
/**
 *Etape 1
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
 *Etape 2
 */
int step2_PowerAmount(char kp){
  if(nb_input>14)return 1;
  if(kp>='0' && kp<='9'){
    lcd.print(kp);
    input[nb_input++]=kp;
    can_go=true;
 }
}

/**
 * Etape 3
 */
int step3_VoltageAmount(char kp){
  if(nb_input>14)return 1;
  if(kp>='0' && kp<='9'){
    lcd.print(kp);
    input[nb_input++]=kp;
    can_go=true;
  }
}
/**
 *Etape 4
 */
int step4_ACDC(char kp){
  if(nb_input>2)return 1;
  if(kp=='A'){
    lcd.print("AC");
    input[nb_input++]='0'+ AC;
    can_go=true;
  }else if (kp=='D'){
    lcd.print("DC");
    input[nb_input++]='0'+ DC;
    can_go=true;
  }

}
/**
 *Etape 4
 */
int step5_Antonome(char kp){
  if(nb_input>14)return 1;
  lcd.print(kp);
  input[nb_input++]=kp;
  can_go=true;
}
/**
 *Etape 5
 */
int step6_Type_Of_Panel(char kp){
  if(nb_input>14)return 1;
  lcd.print(kp);
  input[nb_input++]=kp;
  can_go=true;
}
/**
 *Etape 6
 */
int step7_Type_Of_Batery(char kp){
  if(nb_input>14)return 1;
  lcd.print(kp);
  input[nb_input++]=kp;
  can_go=true;
}
/**
 *Etape 7
 */
int step8_Calculate(){
  double Ir;
 switch(site_number){
   case ALGER: Ir=Ir_Alger;
   break;
   case BOUMERDES:Ir=Ir_BOUMERDES;
   break;
   case ORAN: Ir=Ir_ORAN;
   break;
 }
 Pch=Er/(K*Ir);

 switch(VoltageAmount){
   case _12V:;break;
   case _24V:;break;
   case _48V:;break;
 }
 switch(ACDC){
   case AC:;break;
   case DC:;break;
 }
 switch(Type_Of_Panel){
   case _100watts:Pc=100;break;
   case _200watts:Pc=200;break;
   case _300watts:Pc=300;break;
   case _500watts:Pc=500;break;
 }

 //calculer nombre de module
  Nm=(int)(Pcs/Pc);

 switch(Type_Of_Batery){
   case pas_batterie:;break;
   case batterie_100W:;break;
   case batterie_200W:;break;
   case batterie_200W:;break;
 }
 calculated=true;
 lcd.print("Résulat calculer :)");
}
/**
 *Etape 8
 */

int step9_NemberOfPanel(){
  lcd.print("Nember of Panel :");
  lcd.setCursor(0,1);
  lcd.print(Nm);
};
/**
 *Etape 9
 */
int step10_OnSerie(){

};
/**
 *Etape 10
 */

int step11_OnParalele(){

};
/**
 *Etape 11
 */
int step12_Cost(){

};
/**
 *keypad function
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
 *sutep the matirial
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
 *main project
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
      case 2: step2_PowerAmount(KP);break;
      case 3: step3_VoltageAmount(KP);break;
      case 4: step4_ACDC(KP);break;
      case 5: step5_Antonome(KP);break;
      case 6: step6_Type_Of_Panel(KP);break;
      case 7: step7_Type_Of_Batery(KP);break;
      case 8: step8_Calculate();break;
    }
  }
  last=KP;
  delay(100);
}
