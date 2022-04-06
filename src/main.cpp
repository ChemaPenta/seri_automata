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
 * v0.62RC 2022-04-02,  
 * Primera versión beta en github
 * Seguridad y pantalla
 * 
 * Pasar a botón selector manual/pedal (semi-auto)
 * 
 * position = 0 atras-arriba    bajar=1
 *          = 1 atras-abajo     adelantar=2
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
 * Se añade una pantalla tipo Nextion
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
unsigned long lastDebounceLP = 0;
unsigned long lastDebounceSE = 0;

unsigned long debounceDelay = 60;
unsigned long debounceDelayS = 20;

unsigned long timeValve=0;
unsigned long timePause=0;
unsigned long timePause2=0;
unsigned long timePause3=0;
unsigned long timePause4=0;

//estos dos timepos se deberían poder regular por pantalla...
//unsigned long timePauseVal=700; // pausa en semi-automático ESPERA
//unsigned long PauseValve=500; //tiempo de espera para el cambio de válvulas delante RETENER
unsigned long Espera=0; // pausa en semi-automático ESPERA
unsigned long Retener=0; //tiempo de espera para el cambio de válvulas delante RETENER
unsigned long Despegue=0; //tiempo de espera atrás antes de arrancar DESPEGUE

unsigned long HeartB=0;
uint8_t hbstate=false; //hearbeat
uint8_t cambios=false;

uint8_t movement=0;
uint8_t valves=false;

uint8_t position=0; //posición actual
/*
uint8_t hdirf=false;
uint8_t hdirb=false;
uint8_t vdir=false; //hacia arriba)
uint8_t pedalact=false;
uint8_t manual=true;
uint8_t moveh=false;
uint8_t movehf;
uint8_t movehb;
uint8_t movev=false;
uint8_t movevu;
uint8_t movevd;
*/
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

// Nextion Objects
// (page id, component id, component name)

NexNumber nexespera = NexNumber(1, 6, "time01"); 
NexNumber nexretener = NexNumber(2, 7, "time02"); 
NexNumber nexdespegue = NexNumber(3, 7, "time03"); 
NexDSButton nexsemi = NexDSButton(0,2,"SemiAuto");
NexDSButton nexlimpia =NexDSButton(0,3,"limpieza");

NexTouch *nex_listen_list[] = {
  &nexsemi, &nexlimpia, NULL
};

void semi_pulsado(void *ptr) {
  semiauto = !semiauto;
}

void limpia_pulsado(void *ptr) {
  limpieza = !limpieza;
}

#include "functions.h"

void setup()
{

  //Serial.begin(115200);
  Serial.begin(9600);
  nexInit();

  nexsemi.attachPop(semi_pulsado, &nexsemi);
  nexlimpia.attachPop(limpia_pulsado, &nexlimpia);

  pinMode(endup,INPUT_PULLUP);
  pinMode(enddown,INPUT_PULLUP);
  pinMode(endfront,INPUT_PULLUP);
  pinMode(endback,INPUT_PULLUP);
  digitalWrite(endfront,HIGH);
  digitalWrite(endback,HIGH);
  pinMode(pedal,INPUT_PULLUP);
  pinMode(lbutt,INPUT_PULLUP);
  digitalWrite(lbutt,HIGH);
  pinMode(semibutt,INPUT_PULLUP);
  digitalWrite(semibutt,HIGH);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);


  pinMode(horz,INPUT_PULLUP);
  pinMode(vert,INPUT_PULLUP);

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

  HeartB=millis();

  leemeEEprom();
  nexespera.setValue(Espera);
  nexretener.setValue(Retener);
  nexdespegue.setValue(Despegue);


}



void loop()
{
  //Hearbeat...
  unsigned long currentm=millis();
  if((currentm-HeartB) >= 500){
    HeartB=currentm;
    hbstate=!hbstate;
    digitalWrite(13,hbstate);
  } 

  nexLoop(nex_listen_list);

    //primero los botones
    leebotonera();

    //después leo los endstops
    leestops();

    //esto es debugging
        if(DEBUG){
        if (cambios){
          printa();
          cambios=false;
        } }

    checkbut(); //comprobamos pulsadores y pedal

    //Pasamos a comprobar endstops

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