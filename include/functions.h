/*******************************************************
<<<<<<< HEAD
 * By Chema'22 v1.0
=======
 * By Chema'22 v0.71RC.
>>>>>>> 854a1c7baa9e7c618e861895e965a767f962de38
 * 
 * Autómata para Seritrade
 * 
 * Funciones del programa
 * 
 * los endstops arriba y abajo están a gnd (LOW) en not triguered
 * se desconectan al activarse, como tenemos un pullup
 * en estas entradas, está en HIGH (true) al activarse
 * 
 * los endstops delante/detrás se ponen a gnd en triguered
 * 
 * VF2 => arriba/abajo
 * VF1 => delante/detrás
 *
 *******************************************************
 * TODO hay que agrupar los millis() en una variable
 * local al principio de las funciones  leebotonera
 * y leestops para que los tiempos sean más ajustados
 * DONE
 * *****************************************************/

void leebotonera(){

    unsigned long actual=millis();

  int reading=!digitalRead(pedal); //pedal 
  if(reading != lastP){
      lastDebounceTP=actual;
      lastP=reading;
  }
  if((actual-lastDebounceTP)>=debounceDelay){
      if(reading != pedalval){
          pedalval=reading;
          //cambios=true;
            if(DebugMode) {
                nexpedal.setValue((uint32_t)pedalval);
            }
      }

  }

  reading = !digitalRead(horz); //boton delante/detrás
  //if(hval != lasthval){
  if(reading != lasthval){
      lastDebounceTh=actual;
      lasthval=reading;
  }
  if((actual-lastDebounceTh)>=debounceDelay){
      if (reading != hval){
        hval = reading;
        //lasthval = hval;
        //cambios=true;
      if(DebugMode) {
        nexhor.setValue((uint32_t)hval);
      }           
      }
   
  }
  reading=!digitalRead(vert); //botón subir/bajar

  if(reading != lastvval){
      lastDebounceTv=actual;
      lastvval=reading;
  }
  if((actual-lastDebounceTv)>=debounceDelay){
      if(reading != vval){
          vval=reading;
      if(DebugMode) {
        nexver.setValue((uint32_t)vval);
      }      
      }

  }
    reading=digitalRead(lbutt); //alarma******

   if(reading != lastalarma){
      lastDebounceAL=actual;
      lastalarma=reading;
  }
  if((actual-lastDebounceAL)>=debounceDelay){
      if(reading != alarma){
          alarma=reading;
          //cambios=true;
      if(DebugMode) {
        nexalr.setValue((uint32_t)alarma);
      }             
      }
   
  } 
   reading=!digitalRead(semibutt); //botón semiauto ******pasará a activación********

   if(reading != lastactivado){
      lastDebounceAC=actual;
      lastactivado=reading;
  }
  if((actual-lastDebounceAC)>=debounceDelay){
      if(reading != activado){
          activado=reading;
          //cambios=true;
      if(DebugMode) {
        nexenc.setValue((uint32_t)activado);
      }                
      }

  } 

   reading=!digitalRead(free12); //botón libre de momento

   if(reading != lastfree12){
      lastDebounceF12=actual;
      lastfree12=reading;
  }
  if((actual-lastDebounceF12)>=debounceDelay){
      if(reading != F12){
          F12=reading;
          //cambios=true;
      if(DebugMode) {
        nexfree.setValue((uint32_t)F12);
      }             
      }
   
  } 

    //nexsemiauto;
}
/* void printa()
{
  //Serial.println("********************************************************************");
  Serial.print(" Front: ");
  Serial.print(frontval);
  Serial.print(" Back: ");
  Serial.print(backval);
  Serial.print(" Up: ");
  Serial.print(upval);
  Serial.print(" Dw: ");
  Serial.print(downval);
  Serial.print(" BV: ");
  Serial.print(vval);
  Serial.print(" BH: ");
  Serial.print(hval);
  Serial.print(" P: ");
  Serial.print(pedalval);
  Serial.print(" P2: ");
  Serial.print(pedal2);
  Serial.print(" movement: ");
  switch (movement)
  {
  case PABAJO:
      Serial.print("2DOWN");
      break;
  case PARRIBA:
      Serial.print("2UP");
      break;
  case PALANTE:
      Serial.print("2FRONT");
      break;
  case PATRAS:
      Serial.print("2BACK");
      break;
  case PARADO:
      Serial.print("PARADO");
      break;
  }
  
  Serial.print(" posit: ");
  switch (position)
  {
  case ATRAS_ARRIBA:
      Serial.print("atras-arriba");
      break;
  case ATRAS_ABAJO:
      Serial.print("atras-abajo");
      break;
  case DELANTE_ABAJO:
      Serial.print("delante-abajo");
      break;
  case DELANTE_ARRIBA:
      Serial.print("delante-arriba");
      break;
  }
  Serial.print(" Limp: ");
  Serial.print(limpieza);
  Serial.print(" Semi: ");
  Serial.println(semiauto);
  //delay(2000);
} */
void leestops(){

    unsigned long actual=millis();
  
  int reading=!digitalRead(endup); //Arriba
    //Serial.print(reading);
  if(reading != lastupval){
      lastDebounceUP=actual;
      lastupval=reading;
  }
  if((actual-lastDebounceUP) >= debounceDelayS){
      if(reading != upval){
          upval=reading;
          //cambios=true;
          //position-=1;
          //if(upval) posv=0;
      if(DebugMode) {
        nexenup.setValue((uint32_t)upval);
      }             
      }
   
  }


  //downval=!digitalRead(enddown);
  reading=!digitalRead(enddown); //Abajo
    //Serial.print(reading);
  if(reading != lastdownval){
      lastDebounceDW=actual;
      lastdownval=reading;
  }
  if((actual-lastDebounceDW)>=debounceDelayS){
      if(reading != downval){
          downval=reading;
          //cambios=true;
          //position+=1;
          //if(downval) posv=1;
      if(DebugMode) {
        nexendw.setValue((uint32_t)downval);
      }                
      }

  }



  //(upval || downval) ? movev = false : movev = true;

  //frontval=digitalRead(endfront);
  reading=digitalRead(endfront);
    //Serial.print(reading);
  if(reading != lastfrontval){
      lastDebounceFR=actual;
      lastfrontval=reading;
  }
  if((actual-lastDebounceFR)>=debounceDelayS){
      if(reading != frontval){
          frontval=reading;
          //cambios=true;
          //position+=2;
          //if(frontval) posh=2;
      if(DebugMode) {
        nexenft.setValue((uint32_t)frontval);
      }                
      }

  }



  //backval=digitalRead(endback);
     //reading=digitalRead(free12);
     reading=digitalRead(endback);
    //Serial.println(reading);
  if(reading != lastbackval){
      lastDebounceBK=actual;
      lastbackval=reading;
  }
  if((actual-lastDebounceBK)>=debounceDelayS){
  
      if(reading != backval){
          backval=reading;
          //cambios=true;
          //position-=2;
         // if(backval) posh=0; 

        if(DebugMode) {
            nexenbk.setValue((uint32_t)backval);
        }            
      }
   
  }
  //if (cambios) position=posh+posv;
    position=99;
    if(upval || backval) position=0;
    if(downval) position+=1;
    if(frontval) position+=2;
}
void mueveVF1(uint8_t direc){
    // movemos VF1 hacia direc, true=delante
    digitalWrite(VF1dir,direc);
    digitalWrite(VF1speed,direc);
    digitalWrite(VF1p,activa);
    (direc) ? movement = PALANTE : movement = PATRAS;
    if(DEBUG) Serial.println(movement);
    cambios=true;
    
}
void para(uint8_t motor){
    // si motor=true VF1 else VF2
    if(motor) {
        digitalWrite(VF1p,apaga);
        //moveh=false;
        movement=PARADO;
    }else{
        digitalWrite(VF2p,apaga);
        //movev=false;
        movement=PARADO;
    }
    cambios=true;
}
void mueveVF2(uint8_t direc)
{
    // movemos VF2 hacia direc, true=arriba
    digitalWrite(VF2dir,direc);
    digitalWrite(VF2p,activa);
    if(direc) { 
        movement = PARRIBA;
     }else{
         movement = PABAJO;
     }
    if(DEBUG) Serial.println(movement);
    cambios=true;

}
void valvulas(uint8_t sentido){
    if (sentido){
        // true es estaba imprimiendo asi que vamos a recoger
        digitalWrite(air1,HIGH);
        if(DEBUG) Serial.print(" aire1 apagado ");
        digitalWrite(air2,LOW);
        if(DEBUG) Serial.println("aire2 encendido");
    }else{
        //false es estaba recogiendo así que vamos a printar
        digitalWrite(air1,LOW);
        if(DEBUG) Serial.println("aire1 encendido");
        digitalWrite(air2,HIGH);
        if(DEBUG) Serial.print(" aire2 apagado ");
        
    }
}


void checkbut(void){
 
    if(semiauto){ //el selector manual/semi está en semi
       // if(!movement){   //y estamos parados
       if(movement) position=99;
            // para saber si se acaba de pulsar pedal uso una 2a variable de estado
            if(pedalval){
                if(!pedal2){ //se acaba de pulsar el pedal
                    if (position==ATRAS_ARRIBA){ //está en la posición correcta... empezamos el workflow
                    pedal2=true;
                    mueveVF2(TODOWN);
                    //cambios=true;
                    }
                }


            }else if(pedal2){ //el else hace que no inicie hasta que suelte el pedal
                switch (position) {
                case ATRAS_ARRIBA:
                    //atras arriba fin
                    pedal2=false;
                    break;
                case ATRAS_ABAJO:
                    //atras abajo
                    //Pausa antes de arrancar DESPEGUE
                    if(pausado4){
                        unsigned long currentm=millis();
                        if(currentm-timePause4 >= tiempos.Despegue){ //DESPEGUE
                            mueveVF1(TOFRONT);
                            pausado4 = false;
                        }
                    }else{
                        pausado4=true;
                        timePause4=millis();
                    }
                    
                    break;
                case DELANTE_ARRIBA:
                    //delante arriba
                    //Pausa???????????????????????????
                    if(pausado3){ // ya hemos pasado por esto, comprobar time
                        unsigned long currentm=millis();
                        if(currentm-timePause3 >= tiempos.Retener){ //RETENER
                            mueveVF1(TOBACK);
                            pausado3 = false;
                        }
                    }else{
                        pausado3=true;
                        timePause3=millis();
                    }
                    
                    break;
                case DELANTE_ABAJO:
                    //delante abajo
/*                     if(DEBUG) Serial.print(" estamos para parar ");
                    if(DEBUG) Serial.print(pausado2); */
                    if(pausado2){ // ya hemos pasado por esto, comprobar time
                        unsigned long currentm=millis();
                        //if(DEBUG) Serial.print("estamos esperando");
                        if(currentm-timePause2 >= tiempos.Espera){ //ESPERA
                            mueveVF2(TOUP);
                            pausado2 = false;
                            if(!limpieza) valvulas(true);
                        }
                    }else{
                        //if(DEBUG) Serial.print("estamos pausando");
                        pausado2=true;
                        timePause2=millis();
                    }                    
                    
                    break;
                default:
                    // si entra aquí, estamos en movimiento o 99
                    break;
                }
            }
       // }
    }else{ //no estamos semiauto
        if(hval){
            //Serial.println("pulsado H");
            if(!paradah){ //final recorrido se para
                if(movement==0){ //inicia movimiento
                    switch (position)
                    {
                    case ATRAS_ARRIBA:
                    case ATRAS_ABAJO:
                        mueveVF1(TOFRONT);
                        //cambios=true;
                        break;
                    case DELANTE_ABAJO:
                        //nada bueno aqui
                        break;
                    case DELANTE_ARRIBA:
                        mueveVF1(TOBACK);
                        //cambios=true;
                        break;
                    }
                }
            }
        }else{ 
            if(movement >= 3) para(VF1);
            //para(VF1);
            //if(paradah) 
            paradah=false;
            //cambios=true;
        }
        
        if(vval){
            if(paradav==false){ //final de recorrido quiero que pare
                if(movement==0){ //inicia movimiento
                    //cambios=true;
                    switch (position)
                    {
                    case ATRAS_ARRIBA: 
                    case DELANTE_ARRIBA:
                        mueveVF2(TODOWN);
                        break;
                    case ATRAS_ABAJO: 
                    case DELANTE_ABAJO:
                        mueveVF2(TOUP);
                        break;
                    }
                }

            }
        }else{
            if(movement==1 || movement==2) para(VF2);
            //para(VF2);
            if(paradav) paradav=false;
            //cambios=true;
        }
    }
}


void leemeEEprom() {
  int eeAddress = 0;
  //uint8_t graba=false;
  uint32_t espera, retener, despegue;
  //EEPROM.get( eeAddress, tiempos );
  EEPROM.get( eeAddress, espera);
  tiempos.Espera=espera;
  eeAddress+=sizeof(uint32_t);
  EEPROM.get( eeAddress, retener);
  tiempos.Retener=retener;
  eeAddress+=sizeof(uint32_t);
  EEPROM.get( eeAddress, despegue);
  tiempos.Despegue=despegue;
  

 /*  if (tiempos.Espera << 0 or tiempos.Espera >= 10001) {
    tiempos.Espera = 0;
    graba=true;
  }
  
  if (tiempos.Retener << 0 or tiempos.Retener >= 10001) {
    tiempos.Retener = 0;
    graba=true;
  }

  if (tiempos.Despegue << 0 or tiempos.Despegue >= 10001) {
    tiempos.Despegue = 0;
    graba=true;
  } */
/*   if(graba) {
    EEPROM.put(eeAddress,tiempos);
  digitalWrite(13,true);
  delay(5000);  
  } */

/*   tiempos.Espera = 0;
  tiempos.Retener = 0;
  tiempos.Despegue = 0; */


    //uint32_t dato=tiempos.Espera;
    //uint32_t dato=1450;
    nexespera.setValue(tiempos.Espera);
    //dato=tiempos.Retener;
    nexretener.setValue(tiempos.Retener);
    //dato=tiempos.Despegue;
    nexdespegue.setValue(tiempos.Despegue);
}

void grabaEEprom(int parte) {
  int eeAddress = 0;
  int salto2=sizeof(uint32_t);
  int salto3=salto2*2;
  uint32_t espera, retener, despegue;
  
  switch(parte){
    case 1:
        eeAddress = 0;
        espera=tiempos.Espera;
        EEPROM.put( eeAddress, espera);
    break;
    
    case 2:
        eeAddress = salto2;
        retener=tiempos.Retener;
        EEPROM.put( eeAddress, retener);
    break;

    case 3:
        eeAddress = salto3;
        despegue=tiempos.Despegue;
        EEPROM.put( eeAddress, despegue);        
    break;
  }
  
  //EEPROM.put( eeAddress, tiempos );
  digitalWrite(13,true);
  delay(5000);

}

void leenex() {
  uint32_t number;
  //uint8_t graba=false;
  nexespera.getValue(&number);
  //number=number*100;
  //if (number != tiempos.Espera) { // && number >= 0 && number <= 10000){
    tiempos.Espera = number;
    //graba=true;
  //}
  
  nexretener.getValue(&number);
  //number=number*100;
  //if (number != tiempos.Retener){ // && number >= 0 && number <= 10000){
    tiempos.Retener = number;
    //graba=true;
  //}

    nexdespegue.getValue(&number);
    //number=number*100;
  //if (number != tiempos.Despegue){ // && number >= 0 && number <= 10000){
    tiempos.Despegue = number;
    //graba=true;
  //}
  //if (graba) 
  //grabaEEprom();
}

void semi_pulsado(void *ptr) {
  //semiauto = !semiauto;
  uint32_t dual_state;
/*   nexsemi.getValue(&dual_state);
  if (dual_state==1){
    semiauto = true;
  }else{
    semiauto=false;
  } */
  semiauto=!semiauto;
  dual_state=semiauto;
  nexsemi.setValue(dual_state);

}

void limpia_pulsado(void *ptr) {
  uint32_t dual_state;
/*   
  nexlimpia.getValue(&dual_state);
  limpieza = dual_state;
 */
    limpieza=!limpieza;
    dual_state=limpieza;
    nexlimpia.setValue(dual_state);
}

void tiempo_pulsado(void *ptr) {
  //lee todos los timings y ponlos en las variables y en la EEPROM
  leenex();
}

void debugea(void *ptr){
    //se ha pulsado el botón que nos lleva a la pantalla de debug...
    DebugMode=true;
}

void nodebugea(void *ptr){
    //se ha pulsado el botón que nos saca de la pantalla de debug...
    DebugMode=false;
}

void rele_pulsado(void *ptr){
    uint32_t number;
    //uint8_t number;
    nexrelex.getValue(&number);

    if((number-10) >= 9)
    {
        //encendiendo el rele
        //cmd_rele(number-20,true);
        digitalWrite(reles[number-20],true);

    } else {

        //apagando rele
        //cmd_rele(number-10,false);
        digitalWrite(reles[number-10],false);
    }

}

void pagina0(void *ptr){
    //se ha llegado a la página 0 de la pantalla Nextion
    //modo normal
    DebugMode=false;
    if(iniciando){
        iniciando=false;
    }else{
       // leenex();
    }
}

void pagina5(void *ptr){
    //se ha llegado a la página 5 de la pantalla Nextion
    //modo debug
    DebugMode=true;
}

void deblee(void *ptr){
    leemeEEprom();
    nexdebesp.setValue(tiempos.Espera);
    nexdebret.setValue(tiempos.Retener);
    nexdebdes.setValue(tiempos.Despegue);
}
void debgraba(void *ptr){
    uint32_t num;
    nexdebesp.getValue(&num);
    tiempos.Espera=num;
    grabaEEprom(1);
    nexdebret.getValue(&num);
    tiempos.Retener=num;
    grabaEEprom(2);
    nexdebdes.getValue(&num);    
    tiempos.Despegue=num;
    grabaEEprom(3);
}
void ActualizaNex(){
    nexespera.setValue(tiempos.Espera);
    nexretener.setValue(tiempos.Retener);
    nexdespegue.setValue(tiempos.Despegue);
}
void setespera(void *ptr){
    //se ha pulsado el botón de grabar espera en page1
    uint32_t num;
    nexEsperaSet.getValue(&num);
    tiempos.Espera=num;
    ActualizaNex();
    grabaEEprom(1);
}
void setretener(void *ptr){
    //se ha pulsado el botón de grabar espera en page1
    uint32_t num;
    nexRetenerSet.getValue(&num);
    tiempos.Retener=num;
    ActualizaNex();
    grabaEEprom(2);
}
void setdespegue(void *ptr){
    //se ha pulsado el botón de grabar espera en page1
    uint32_t num;
    nexDespegueSet.getValue(&num);
    tiempos.Despegue=num;
    ActualizaNex();
    grabaEEprom(3);
}
