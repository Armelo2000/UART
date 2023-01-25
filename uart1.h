
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
* Diese Routine empfängt einen String mit der gegebene Länge 
* über den UART shnittstelle
********************************************************************/
void receive_StringD(char* str, int length);

/*******************************************************************
* Diese Routine empfängt einen Charakter und sendet zurück an den PC
********************************************************************/
void Echo();


#endif