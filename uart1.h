
#ifndef _UART_1_H
#define _UART_1_H

/**************************************************************
* Diese Routine prüft ob der UART bereit ist zu senden
**************************************************************/
int isReadyToSend();

/**************************************************************
* Diese Routine prüft ob der UART bereit ist zu empfangen
**************************************************************/
int isReadyToReceive();

/*******************************************************************
* Diese Routine schreibt in einem beliebigen Register
********************************************************************/
void writeTo(void* ptAddr, unsigned int val);

/*******************************************************************
* Diese Routine liest von einem beliebigen Register
********************************************************************/
unsigned int readFrom(void* ptAddr);

/*******************************************************************
* Diese Routine empfängt einen Charakter bis der charackter ch1
* gesendet ist
********************************************************************/
void receive_CharUntil(char* str, char ch1);

/*******************************************************************
* Diese Routine empfängt einen String bis die charackter ch1
* gesendet ist
********************************************************************/
int receive_StringUntil(char* str, char ch1);
  
/******************************************************************
* Diese Routine sendet einen Charakter über den UART shnittstelle
*******************************************************************/
void UART1_Send_Char(char ch);

/*******************************************************************
* Diese Routine empfängt einen Charakter über den UART shnittstelle
********************************************************************/
unsigned char UART1_Receive();

/*******************************************************************
* Diese Routine sendet einen String über den UART shnittstelle
********************************************************************/
void UART1_Send_Str(char* ptCh);

/*******************************************************************
* Diese Routine sendet einen String mit definierte länge über den 
* UART shnittstelle
********************************************************************/
void UART1_SendString_Len(char* ptCh, unsigned int len);
  
/*******************************************************************
* Diese Routine empfängt einen String mit der gegebene Länge 
* über den UART shnittstelle
********************************************************************/
void receive_StringD(char* str, int length);

/*******************************************************************
* Diese Routine empfängt einen Charakter und sendet zurück an den PC
********************************************************************/
void Echo();

/*******************************************************************
* Diese Routine konvertiert hex zahl zu char array
********************************************************************/
int hexToCharArry(unsigned int hex, char* ptChar);

/*******************************************************************
* Diese Routine konvertiert char array zu hex Zahl
********************************************************************/
int charArryToHex(char *ptChar, unsigned int *hex);

/*******************************************************************
* Diese Routine konvertiert char array zu einem Dezimalwert
********************************************************************/
int charArryToValue(char *ptChar, unsigned int *hex, int length);

/*******************************************************************
* Diese Routine kopiert von einem Quelle zu einem andere bis das
* Endzeichen endeckt ist.
********************************************************************/
void copyUntil(void* dst, void* src, char endCahr);

/*******************************************************************
* Diese Routine kopiert Daten von einem Quelle zu einem andere bis das
********************************************************************/
void copy(void* dst, void* src, int len);

#endif
