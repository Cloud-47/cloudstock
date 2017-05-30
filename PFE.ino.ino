//make a function to store keyvalue and KeyPressed
#include <LiquidCrystal.h>
#include <string.h>
//Constants
//Coefficient correcteur
#define K 0.55
//Coefficient de pert
#define Cp  0.8
//
#define Rt 0.8
#define Pd 0.8
//rondemenet
#define Rond 0.9


//Courant
#define AC 0
#define DC 1
//SITE
#define ALGER     1
#define BOUMERDES 2
#define ORAN     3
//Ensoleiment
#define Ne_Alger 100
#define Ne_BOUMERDES 120
#define Ne_ORAN 90
//Volatge utilisé
#define _12V 1
#define _24V 2
#define _48V 3

//Type de panneau
#define _100watts  1
#define _200watts  2
#define _300watts  3
#define _500watts  5
//Cost panneau;
#define cost_100watts  10000
#define cost_200watts  20000
#define cost_300watts  30000
#define cost_500watts  50000
//type de batterie
#define pas_batterie  0
#define batterie_100W 1
#define batterie_200W 2
#define batterie_500W 5
//Cost Battery
#define Cost_batterie_100W 10000
#define Cost_batterie_200W 20000
#define Cost_batterie_500W 50000

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

bool calculated=false;
//
char clean_row[]="                ";

//Valeur entrer par l'utilisateur
byte site_number;
double Ec;//Energie consommée
byte temps=12;//temps de fonctionement par heure
byte VoltageAmount;
byte ACDC;
byte Type_Of_Panel;
double Antonome=1;
byte Type_Of_Batery;


//Valeur calculer par le programme
double Pch;//Puissance crete
double Vch;//tension utilisée par la charge
double Vn;// tension nominal
double Pc;//Puissance des panneaux
double cost;
int Nm;//Nombre de module
int Ns;//Nombre de module en série
int Np;//nombre de module en parallel
int Nb;//Nombre de batterie


/**
 *affichage de parapatre sur LCDon
 */
int print_step(){
  lcd.clear();
  lcd.setCursor(0,0);
  switch(step){
      case 1: lcd.print("Enter the SITE");break;
      case 2: lcd.print("Enter the Power Amount");break;
      case 3: lcd.print("Enter the Volate Amount");break;
      case 4: lcd.print("AC or DC");break;
      case 5: lcd.print("Autonomie");break;
      case 6: lcd.print("Type Of Panel");break;
      case 7: lcd.print("Type Of Battery");break;
      case 8: step8_Calculate();break;
      case 9: step9_NemberOfPanel();break;
      case 10: step10_OnSerie();break;
      case 11: step11_OnParalele();break;
      case 12: step12_battry();break;
      case 13: step13_Cost();break;
      case 14:  lcd.print("New Calculation"); delay(2000);
                init_step();break;
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
    input[nb_input++]='0';
    can_go=true;
  }else if (kp=='D'){
    lcd.print("DC");
    input[nb_input++]='';
    can_go=true;
  }

}
/**
 *Etape 4
 */
int step5_Antonome(char kp){
  if(nb_input>14)return 1;
  if(kp=='D'){
    lcd.print('.');
    input[nb_input++]='.';
      can_go=false;
  }else{
    lcd.print(kp);
    input[nb_input++]=kp;
    can_go=true;
  }

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
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calulating...");

  double Bj;
  switch(ACDC){
    case AC:Bj=Ec*temps/Rond;break;
    case DC:Bj=Ec*temps;break;
  }

  double Ne;
 switch(site_number){
   case ALGER: Ne=Ne_Alger;
   break;
   case BOUMERDES:Ne=Ne_BOUMERDES;
   break;
   case ORAN: Ne=Ne_ORAN;
   break;
   default : Ne=Ne_Alger;
 }

//Calcule Pch
 Pch=Bj/(Cp*Ne);

 switch(VoltageAmount){
   case _12V:Vch=12;break;
   case _24V:Vch=24;break;
   case _48V:Vch=48;break;
   default : Vch=48;
 }
 int cost_1pann;
 switch(Type_Of_Panel){
   case _100watts:Pc=100;Vn=12;cost_1pann=cost_100watts;break;
   case _200watts:Pc=200;Vn=12;cost_1pann=cost_200watts;break;
   case _300watts:Pc=300;Vn=24;cost_1pann=cost_300watts;break;
   case _500watts:Pc=500;Vn=48;cost_1pann=cost_500watts;break;
 }

 //calculer nombre de module
  Nm=(int)(Pch/Pc);
 // En série et en parallel
  Ns=(int)(Vch/Vn) +1;
  Np=(int)(Nm/Ns) +1;



  double Cbat=Bj*Antonome/(Vch*Rt*Pd);
  int C1bat;
  int cost_1batt;
 switch(Type_Of_Batery){
   case batterie_100W:C1bat=100;cost_1batt=Cost_batterie_100W;break;
   case batterie_200W:C1bat=200;cost_1batt=Cost_batterie_200W;break;
   case batterie_500W:C1bat=500;cost_1batt=Cost_batterie_500W;break;
 }
 if(Type_Of_Batery==pas_batterie)
    Nb=0;
  else
    Nb=(int)(Cbat/C1bat) +1;

 // Calculer le cout
 cost=Nb*cost_1batt + Nm*cost_1pann;




 calculated=true;
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Done.");
 lcd.setCursor(0,1);
 lcd.print("Press # to print :)");
}
/**
 *Etape 9
 */

int step9_NemberOfPanel(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Number of Panel :");
  lcd.setCursor(0,1);
  lcd.print(Nm);
};
/**
 *Etape 10
 */
int step10_OnSerie(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Number of serial Panel :");
  lcd.setCursor(0,1);
  lcd.print(Ns);
};
/**
 *Etape 11
 */

int step11_OnParalele(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Number of Parallel Panel :");
  lcd.setCursor(0,1);
  lcd.print(Np);
};
/**
 * Etape 12
 */
int step12_battry(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Number of battry :");
  lcd.setCursor(0,1);
  lcd.print(Nb);
};
/**
 *Etape 13
 */
int step13_Cost(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cost :");
  lcd.setCursor(0,1);
  lcd.print(cost);
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
