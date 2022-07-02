/***************************************************
 * SeriAuto programa a ejecutar en Arduino Nano
 * para controlar equipo de serigrafía 
 * Copyright (C) 2022  Chema Muñoz

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * v1.00 2022-08-28,  
 * Primera versión electrónica PCB comercial
 * 
 * reajustar I/O para nuevo esquema electrónico
 * 
 * position = 0 atras-arriba    
 *          = 1 atras-abajo     
 *          = 3 delante-abajo   
 *          = 2 delante-arriba
 * las posiciones vienen por los movimientos 
 * generados, no sólo por los sensores
 * 
 * movement = 0 parado
 *          = 1 pabajo
 *          = 2 parriba
 *          = 3 palante
 *          = 4 patras
 * 
 * Los variadores de frecuencia tienen unos pines
 * de registro de errores (stall?)
 * debería revisarlos
 * A falta de eso, todo lo demás ya funciona 
 * Estamos en proceso de pruebas en la máquina
 * de Seritrade, un par de semanas y a producción
 * 
 * Pantalla tipo Nextion
 * por puerto serie para configurar las pausas
 * además de grabar estas
 * configuraciones en EEPROM
 * *************************************************/

#include <Arduino.h>
#include "config.h"

unsigned long lastDebounceTh = 0;
unsigned long lastDebounceTv = 0;
unsigned long lastDebounceTP = 0;

unsigned long lastDebounceUP = 0;
unsigned long lastDebounceDW = 0;
unsigned long lastDebounceFR = 0;
unsigned long lastDebounceBK = 0;
unsigned long lastDebounceAL = 0;
unsigned long lastDebounceAC = 0;
unsigned long lastDebounceF12 = 0;

unsigned long debounceDelay = 60;
unsigned long debounceDelayS = 20;

unsigned long timeValve=0;
unsigned long timePause=0;
unsigned long timePause2=0;
unsigned long timePause3=0;
unsigned long timePause4=0;

tiempos_STR tiempos;

unsigned long HeartB=0;
uint8_t hbstate=false; //hearbeat
uint8_t cambios=false;

uint8_t movement=0;
uint8_t valves=false;

uint8_t position=99; //posición actual 99=desconocido/en movimiento
uint8_t DebugMode=false;

uint32_t limpieza=false; //selector limpieza
//uint8_t lastlimpieza=false;
uint8_t pedal2=false;
uint8_t paradah=false;
uint8_t paradav=false;

uint8_t upval;
uint8_t downval;
uint8_t frontval;
uint8_t backval;

uint8_t lastupval;
uint8_t lastdownval;
uint8_t lastfrontval;
uint8_t lastbackval;

uint8_t alarma=false;
uint8_t lastalarma;
uint8_t EstadoAlarma=false;

uint8_t iniciando=true;

uint8_t activado=true;
uint8_t lastactivado;

uint8_t F12;
uint8_t lastfree12;

uint8_t pedalval=false; //botón pedal
uint32_t semiauto = false; //selector semiauto/manual
//uint8_t lastsemiauto; //selector semiauto/manual
uint8_t lastP=false; //intermedio pedal
uint8_t hval=false;     //botón horizontal
uint8_t vval=false;     //botón vertical
uint8_t lasthval=false;     //botón horizontal anterior
uint8_t lastvval=false;     //botón vertical anterior

uint8_t pausado=false;
uint8_t pausado2=false;
uint8_t pausado3=false;
uint8_t pausado4=false;

uint8_t reles[9] = {0,9,8,7,6,5,4,3,2};

// Nextion Objects
// (page id, component id, component name)

NexNumber nexespera = NexNumber(0, 6, "t1"); 
//NexNumber nexespera = NexNumber(1, 7, "t4"); 
NexNumber nexretener = NexNumber(0, 7, "t2"); 
NexNumber nexdespegue = NexNumber(0, 8, "t3");

NexNumber nexEsperaSet = NexNumber(1,7,"t4");
NexNumber nexRetenerSet = NexNumber(2,7,"t2");
NexNumber nexDespegueSet = NexNumber(3,7,"t3");

NexDSButton nexsemi = NexDSButton(0, 2, "s");
NexDSButton nexlimpia =NexDSButton(0, 3, "l");
NexButton nexsalir_espera = NexButton(1, 3, "b3");
NexButton nexdebug = NexButton(1, 5, "b4");
NexButton nexnodebug = NexButton(5, 1, "b3");
NexButton nexsalir_retener = NexButton(2, 3, "b3");
NexButton nexsalir_despegue = NexButton(3, 3, "b3");
NexButton nexdeblee = NexButton(5, 43, "b1");
NexButton nexdebgraba = NexButton(5, 44, "b2");

NexButton nexsetespera = NexButton(1, 9, "b5");
NexButton nexsetretener = NexButton(2, 8, "b5");
NexButton nexsetdespegue = NexButton(3, 8, "b5");

NexPage Alarm = NexPage(4, 0, "Alarm");
NexPage pral = NexPage(0, 0, "page0");
NexRadio nexpedal = NexRadio(5, 2, "r0");
NexRadio nexhor = NexRadio(5, 3, "r1");
NexRadio nexver = NexRadio(5, 4, "r2");
NexRadio nexendw = NexRadio(5, 5, "r3");
NexRadio nexenup= NexRadio(5, 6, "r4");
NexRadio nexenft = NexRadio(5, 7, "r5");
NexRadio nexenbk = NexRadio(5, 8, "r6");
NexRadio nexenc = NexRadio(5, 9, "r7");
NexRadio nexalr = NexRadio(5, 10, "r8");
NexRadio nexfree = NexRadio(5, 11, "r9");
NexCheckbox nexrel1 = NexCheckbox(5, 22, "c0");
NexCheckbox nexrel2 = NexCheckbox(5, 23, "c1");
NexCheckbox nexrel3 = NexCheckbox(5, 24, "c2");
NexCheckbox nexrel4 = NexCheckbox(5, 25, "c3");
NexCheckbox nexrel5 = NexCheckbox(5, 26, "c4");
NexCheckbox nexrel6 = NexCheckbox(5, 27, "c5");
NexCheckbox nexrel7 = NexCheckbox(5, 28, "c6");
NexCheckbox nexrel8 = NexCheckbox(5, 29, "c7");
NexNumber nexrelex = NexNumber(5, 39, "n0");
NexNumber nexdebesp = NexNumber(5, 40, "n1");
NexNumber nexdebret = NexNumber(5, 41, "n2");
NexNumber nexdebdes = NexNumber(5, 42, "n3");

NexPage nexpage0 = NexPage(0,0,"page0");
NexPage nexpage5 = NexPage(5,0,"debug");

NexTouch *nex_listen_list[] = {
  &nexsemi, &nexlimpia, &nexdebug, &nexnodebug, &nexsalir_espera, &nexsalir_retener, &nexsalir_despegue, 
  &nexrel1, &nexrel2, &nexrel3, &nexrel4, &nexrel5, &nexrel6, &nexrel7, &nexrel8, 
  &nexpage0, &nexpage5, 
  &nexdeblee, &nexdebgraba,
  &nexsetespera, &nexsetretener, &nexsetdespegue, NULL
};

#include "functions.h"

void setup()
{

 /*  tiempos.Espera = 100;
  tiempos.Despegue = 200;
  tiempos.Retener = 300;
 */
  //Serial.begin(115200);
  Serial.begin(9600);
  nexInit();

  nexsemi.attachPop(semi_pulsado, &nexsemi);
  nexlimpia.attachPop(limpia_pulsado, &nexlimpia);
  nexdebug.attachPop(debugea, &nexdebug);
  nexnodebug.attachPop(nodebugea, &nexnodebug);
  nexsalir_espera.attachPop(tiempo_pulsado, &nexsalir_espera);
  nexsalir_retener.attachPop(tiempo_pulsado, &nexsalir_retener);
  nexsalir_despegue.attachPop(tiempo_pulsado, &nexsalir_despegue);
  nexrel1.attachPop(rele_pulsado, &nexrel1);
  nexrel2.attachPop(rele_pulsado, &nexrel2);
  nexrel3.attachPop(rele_pulsado, &nexrel3);
  nexrel4.attachPop(rele_pulsado, &nexrel4);
  nexrel5.attachPop(rele_pulsado, &nexrel5);
  nexrel6.attachPop(rele_pulsado, &nexrel6);
  nexrel7.attachPop(rele_pulsado, &nexrel7);
  nexrel8.attachPop(rele_pulsado, &nexrel8);
  nexpage0.attachPop(pagina0, &nexpage0);
  nexpage5.attachPop(pagina5, &nexpage5);
  nexdebgraba.attachPop(debgraba,&nexdebgraba);
  nexdeblee.attachPop(deblee,&nexdeblee);

  nexsetespera.attachPop(setespera,&nexsetespera);
  nexsetretener.attachPop(setretener,&nexsetretener);
  nexsetdespegue.attachPop(setdespegue,&nexsetdespegue);

  pinMode(endup,INPUT_PULLUP);
  pinMode(enddown,INPUT_PULLUP);
  pinMode(endfront,INPUT_PULLUP);
  pinMode(endback,INPUT_PULLUP);
  //digitalWrite(endfront,HIGH);
  //digitalWrite(endback,HIGH);
  pinMode(pedal,INPUT_PULLUP);
  pinMode(lbutt,INPUT_PULLUP);
  digitalWrite(lbutt,HIGH);
  pinMode(semibutt,INPUT_PULLUP);
  digitalWrite(semibutt,HIGH);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);


  pinMode(horz,INPUT_PULLUP);
  pinMode(vert,INPUT_PULLUP);

 pinMode(free12,INPUT_PULLUP);

  pinMode(VF1p,OUTPUT);
  pinMode(VF1dir,OUTPUT);
  pinMode(VF1speed,OUTPUT);
  digitalWrite(VF1p,HIGH);
  digitalWrite(VF1dir,HIGH);
  digitalWrite(VF1speed,HIGH);

  pinMode(VF2p,OUTPUT);
  pinMode(VF2dir,OUTPUT);
  digitalWrite(VF2p,HIGH);
  digitalWrite(VF2dir,HIGH);

  pinMode(air1,OUTPUT);
  digitalWrite(air1,HIGH);
  pinMode(air2,OUTPUT);
  digitalWrite(air2,HIGH);

  pinMode(free8,OUTPUT);
  digitalWrite(free8,HIGH);

  //HeartB=millis();

  leemeEEprom();

}

void loop()
{
  
  nexLoop(nex_listen_list);

  //Hearbeat...
  unsigned long currentm=millis();
  unsigned long latido;
  
  (limpieza || semiauto || DebugMode) ? latido = 80 : latido = 800;
  
  if((currentm-HeartB) >= latido){
    HeartB=currentm;
    hbstate=!hbstate;
    digitalWrite(13,hbstate);
  } 

    //primero los botones
    leebotonera();

    //agrego el tema del debug
    if(DebugMode){

      //cargo los datos de botones y demás...
      leestops();
      //checkbut();
      //ahora a pasarlos al Nextion .............................



    } else {

        if(activado){ //botón de activación encendido, procedemos
          if(EstadoAlarma){
              //ha saltado la alarma loop sin hacer otra cosa que subir
              
              leestops();
              
              if(upval){
                para(VF2);
              }else{
                mueveVF2(TOUP);
              }

          }else{ //no hay estado de alarma, actuamos normal

            if(alarma){ //Salta la alarma

              EstadoAlarma=true;
              Alarm.show();
              para(VF1);
              mueveVF2(TOUP);

            }else{ //No hay alarma, así que normal
            
              //leo los endstops
              leestops();



              checkbut(); //comprobamos pulsadores y pedal

              if(upval) {
                  //position-=1;
                  if(semiauto==0) if(vval) paradav=true;
                  if(movement==PARRIBA) para(VF2);
              }

              if(downval){
                  //position+=1;

                  if(semiauto==0) if(vval) paradav=true;
                  if(movement==PABAJO) para(VF2);
              }

              if(frontval){
                  //position+=2;
                  if(movement==PALANTE){ 
                    para(VF1);
                  }

                  if (semiauto==0) {
                    if(hval) paradah=true;
                  if(limpieza==0) {
                    valvulas(true);
                  }}
              }
          
              if(backval){
                  //position-=2;
                  if(movement==PATRAS) para(VF1);
                  valvulas(false);
                  if (semiauto==0) if(hval) paradah=true;
              }

            } 
          } 
        } else { //Botón de activación en OFF, nada se mueve
          //por si acaso se desactiva durante movimiento, paramos motores
          para(VF1);
          para(VF2);

          if(EstadoAlarma) pral.show();

          EstadoAlarma=false;
          
        }
    }
}