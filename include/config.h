/***********************************************************************************
 *     Primera versión autómata serigrafía 
 * by Chema'22 v1.0
 * 
 *  Cambios en el I/O para usar la PCB comercial VERDE
 * 
 *  Pantalla Nextion para ajustar valores de pausas
 *  podría ser otra pantalla HMI, pero esta estaba a mano
 * 
 * *********************************************************************************
 * Tods (o casi) los I/O pins han variado en la v1.0 al crear la nueva PCB
 * las salidas de relé van de D9 a D2 (relé K1 al K8)
 * y casi todas las entradas han ido a puertos analógicos
 * *********************************************************************************
 * 
 * dos variadores de frecuencia 
 * VF1 pequeño (1HP) mueve delante detrás
 * con D2 enciendo o apago VF1 (por relé)                               K1 (blanco)
 * con D4 selecciono dirección (por relé)                               K2 (gris)
 * con D6 cambio potenciómetro de velocidad (impresión/entintado)       K3 (lila)
 * FALTARÍA gestionar si VF1 está ok o no...
 * 
 * VF2 grande (2HP) mueve arriba abajo
 * con D8 enciendo/apago VF2                                            K4 (Azul)
 * con D10 selecciono dirección                                         K5 (verde)
 * 
 * D3 esta a gnd si no está activado (final de carrera arriba VF2)      [1]
 * D5 esta a gnd si no está activado (final de carrera abajo VF2)       [2]
 * 
 * D7 esta a gnd si está activado (final de carrera delante VF1)        [3]
 * D9 esta a gnd si está activado (final de carrera detrás VF1)         [4]
 * 
 * D11 activa por relé un juego de válvulas de aire (1)                 K6 (amarillo)
 * D12 activa por relé un juego de válvulas de aire (2)                 K7 (naranja)
 * 
 * libre                                                                K8 (rojo)
 * Vcc                                                                  K0 (marrón)
 * Gnd                                                                  K9 (negro)
 * 
 * A0 se pone en gnd al pulsar el pedal de inicio maniobra              [0]
 * 
 * A1 se pone a gnd al pulsar el botón de flechas horizontales          [10]
 * A3 se pone a gnd al pulsar el botón de flechas verticales            [6]
 * A2 se pone a gnd al activar botón limpieza                           [    ]
 * A4 se pone a gnd al activar sistema semi-automático                  [ ]
 * 
 * *********************************************************************************/

#include <EEPROM.h>
#include <Nextion.h>

#define activa LOW
#define apaga HIGH

#define DEBUG false              //para mandar más o menos mensajes por Serial

#define VF1p 9 //2  //2             [K1]
#define VF1dir 8 //4 //8            [K2]
#define VF1speed 7 //6 //13         [K3]
#define VF1 true
#define VF2 false

#define VF2p 6 //8 //7              [K4]
#define VF2dir 5 //10 //9           [K5]

#define endup 11 //5 //6             [2]
#define enddown 10 //3 //5           [1]

#define endfront 12 //7 //10         [3]
#define endback A5 //9 //11          [4]

#define air1 4 //11 //3             [K6]
#define air2 3 //12 //12            [K7]
#define free8 2 //                  [K8]

#define pedal A0 //4                 [0]
#define lbutt A2 //A2                [99]  ALARMA tras poner pantalla Nextion
#define horz A1 //A0                 [10]
#define vert A3 //A1                 [9]
#define semibutt A4  //              [69] control de encendido
#define free12 A6 //                 [12] libre

#define TOFRONT true
#define TOBACK false

#define TOUP true
#define TODOWN false

#define RECOGER true
#define PRINTAR false

#define PALANTE 3
#define PATRAS 4
#define PARRIBA 2
#define PABAJO 1
#define PARADO 0

#define DELANTE_ABAJO 3
#define DELANTE_ARRIBA 2
#define ATRAS_ARRIBA 0
#define ATRAS_ABAJO 1

#define PAUSA 1000 //cuanto pausa al llegar arriba desde impresión ms

struct tiempos_STR{
  uint32_t Espera;
  uint32_t Retener;
  uint32_t Despegue;
};
