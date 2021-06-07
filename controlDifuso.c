//LCD
sbit LCD_RS at RD5_bit;
sbit LCD_EN at RD4_bit;
sbit LCD_D4 at RD0_bit;
sbit LCD_D5 at RD1_bit;
sbit LCD_D6 at RD2_bit;
sbit LCD_D7 at RD3_bit;
sbit LCD_RS_Direction at TRISD5_bit;
sbit LCD_EN_Direction at TRISD4_bit;
sbit LCD_D4_Direction at TRISD0_bit;
sbit LCD_D5_Direction at TRISD1_bit;
sbit LCD_D6_Direction at TRISD2_bit;
sbit LCD_D7_Direction at TRISD3_bit;

unsigned int sensor, kcentro;

char *text;

// VARIABLES
double A1, A2, A3, NEG, CEN, POS;
double C1, C2, C3;
double hecho;
int dif,prueba;
double K[ 3 ][ 3 ], Cs[ 3 ][ 3 ] , DD[ 3 ][ 3 ];
int i, j;
double num, den, yc;

//salida
unsigned char txt;
unsigned char txtUno;
unsigned int yc_pwm;

//Sensor
void distancia(void){

sensor = ADC_Read(0); //ADC 16 bits
sensor = ADRESH; //necesita dos registros L/H
sensor = (sensor * 255);
sensor = sensor+ ADRESL;
sensor= (6050 / sensor); //[cm] math[0-99cm] d=1/volts; escalando(6050)
if(sensor >= 2) sensor =sensor - 2; // error lineal (-2)
if(sensor < 10) sensor = 10;
if(sensor > 80) sensor = 80;

}

//Función de entrada e
double FUNCIONA( double val , int op ) {
double a,b, res;
if(op == 1 ) {

a = 18.; b = 0.1; //Modificar para Ajustar
if (val >= a)
res = 0.;
else
res = 1. - exp( -b * ( ( a - val )* ( a - val)) ) ;

}

else if(op == 2){
a = 20.; b = .1; //Modificar para Ajustar
res = exp( -b*((val-a)*(val-a)) ) ;

}

else {

a = 22.; b = 0.1; //Modificar para Ajustar
if (val <= a)
res = 0.;
else
res = 1. - exp( -b *((val-a)*(val-a))) ;
}

return res;
}


//FUNCION PARA OBTENER NEG, CEN , POS e negada
double FUNCIONBARRA( double val , int op ) {

double a,b,res;

if(op == 1 ) {

a = -2.; b = .1; //Modificar para Ajustar
if (val >= a)
res = 0.;
else
res = 1. - exp( -b *(( a-val )*( a-val ))) ;
}
else if(op == 2){
a = 0.; b = .1; //Modificar para Ajustar
res = exp( -b* ((val-a )*( val-a )));

}

else {
a = 2.; b = 0.1; //Modificar para Ajustar
if (val <= a)
res = 0;
else
res = 1. - exp( -b * (( val-a )*( val-a )) ) ;
}
return res;
}


//FUNCION HECHO
double FUNCIONH ( double val ) {

double a,b,res;
a = 20.; b = 0.051; //Modificar para Ajustar
res = exp( -b * (( val-a )*( val-a )) );
return res;

}


void main(void)
{

//ADCON1 = 0X0F;

OSCCON = 0x66;
ADCON0 = 0X01; // RA0 analogica
ADCON1 = 0X0E;
TRISA0_bit = 1; //entrada

PORTA = 0x00; //inicializada en 0
PORTC = 0;
TRISC = 0;

PWM2_Init(5000);
PWM2_Start();

PORTB = 0;
TRISB = 0;

LCD_Init();
LCD_Cmd(_LCD_CURSOR_OFF);
LCD_Cmd(_LCD_CLEAR);

UART1_Init(9600);

while(1){

distancia();

WordToStrWithZeros(sensor,txt); //printf distancia

Lcd_Out(1,1,txt); //(double) sensor; //int-unsigned to double

kcentro = 19; // Valor propuesto para k

C1 = 245;//248;//220;//115;//63;//75;
C2 = 252;//252;//230;//140;//97;//120;
C3 = 256;//256;//247;//205;//125;//163;

PORTB.F0= 1;

//e
A1 = FUNCIONA ( sensor, 1 ); //Valor de Pertenencia para el Conjunto A1 double
A2 = FUNCIONA ( sensor, 2 ); //Valor de Pertenencia para el Conjunto A2
A3 = FUNCIONA ( sensor, 3 ); //Valor de Pertenencia para el Conjunto A3

//e negada
dif = kcentro - sensor; //La diferencia (int) entre el Valor de K y el obtenido int-unsigned
NEG = FUNCIONBARRA( dif, 1 ); // Valores para NEG double
CEN = FUNCIONBARRA( dif, 2 ); // Valores para CEN
POS = FUNCIONBARRA( dif, 3 ); // Valores para POS

//Hecho
hecho = FUNCIONH ( sensor ) ; // Valor de Pertenencia del Hecho A' double

//FloatToStr(hecho,txt); //printf hecho
//Lcd_Out(2,1,txt);
K[0][0] = 0; K[0][1] =(A1 < CEN)? A1: CEN; K[0][2] = (A1 < POS)? A1: POS; //reglas double
K[1][0] = (A2 < NEG)? A2: NEG; K[1][1] =(A2 < CEN)? A2: CEN; K[1][2] = (A2 < POS)? A2: POS;
K[2][0] = (A3 < NEG)? A3: NEG; K[2][1] =(A3 < CEN)? A3: CEN; K[2][2] = 0 ;
Cs[0][0] = 0; Cs[0][1] = C1; Cs[0][2] = C1; //constantes

CENTROS DE GRAVEDAD
Cs[1][0] = C2;//C1;
Cs[1][1] = C2;
Cs[1][2] = C2;//C3;
Cs[2][0] = C3; Cs[2][1] = C3; Cs[2][2] = 0 ;

for(i=0; i<3; i++ ){

for (j=0; j<3; j++) {

DD[i][j] = (K[i][j] < hecho)? K[i][j]: hecho;

}

}
num = den = 0 ;
for(i=0; i<3; i++ ){

for (j=0; j<3; j++) {
num += DD[i][j] * Cs[i][j];
den += DD[i][j];

}

}

yc=num/den;
yc_pwm=yc; //3-100
PWM2_Set_Duty(yc_pwm);
IntToStr(yc_pwm,txt); //printf

}

}