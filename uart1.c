
#include "uart1.h"

/**************************************************************
* Diese Routine prüft ob der UART bereit ist zu senden
**************************************************************/
int isReadyToSend(){

	if(U1LSR & 0x20){
		return 1;
	}else{
		return 0;
	}

}

/**************************************************************
* Diese Routine prüft ob der UART bereit ist zu empfangen
**************************************************************/
int isReadyToReceive(){

	if(U1LSR & 0x01){
		return 1;
	}else{
		return 0;
	}

}

/******************************************************************
* Diese Routine sendet einen Charakter über den UART shnittstelle
*******************************************************************/
void UART1_Send_Char(char ch)
{
	// Wait for Previous transmission
    while(isReadyToSend()); 
	
	// Load the data to be transmitted
    U1THR=ch;                        
}

/*******************************************************************
* Diese Routine empfängt einen Charakter über den UART shnittstelle
********************************************************************/
unsigned char UART1_Receive()
{
 char ch; 
 // Wait till the data is received
    while(isReadyToReceive()); 
	
    ch = U1RBR;                                
 return ch;
}

/*******************************************************************
* Diese Routine sendet einen String über den UART shnittstelle
********************************************************************/
void UART1_Send_Str(char* ptCh)
{
	//prüfen dass der Pointer nicht NULL ist
	//falls es NULL ist, soll die Funktion sofort verlassen
	if(ptCh == NULL) return;
	
	while(*ptCh != '\0'){
			
		UART1_Send_Char(*ptCh);
		ptCh++;
	}                     
}

/*******************************************************************
* Diese Routine empfängt einen String mit der gegebene Länge 
* über den UART shnittstelle
********************************************************************/
void receive_StringD(char* str, int length)
{
	//prüfen dass der Pointer nicht NULL ist
	//falls es NULL ist, soll die Funktion sofort verlassen
	if(str == NULL) return;	
	
	while(len--){
		*str++ = UART1_Receive();
	}
	
}

void Echo()
{
	char ch = UART1_Receive();
	
	UART1_Send_Char(ch);
	
}