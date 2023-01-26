

#include "uart1.h"

const unsigned int BaudRate = 19200u;
const unsigned int PCLK = 12500000u;

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
	if(ptCh == (void *)0) return;
	
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
	if(str == (void *)0) return;	
	
	while(length--){
		*str++ = UART1_Receive();
	}
	
}

/*******************************************************************
* Diese Routine empfängt einen String bis das Kommando CR => 0x0D 
* gesendet ist
********************************************************************/
int receive_String(char* str)
{
	//prüfen dass der Pointer nicht NULL ist
	//falls es NULL ist, soll die Funktion sofort verlassen
	if(str == (void *)0) return;	
	
	char ch = 0x00;
	int counter = 0;
	
	while(ch != 0x0D){
		
		ch = UART1_Receive();
		
		*str++ = ch;
		counter++;
	}
	return counter;
}

/*******************************************************************
* Diese Routine empfängt einen String bis die charackter ch1 oder ch2 
* gesendet ist
********************************************************************/
int receive_StringUntil(char* str, char ch1, char ch2)
{
	//prüfen dass der Pointer nicht NULL ist
	//falls es NULL ist, soll die Funktion sofort verlassen
	if(str == (void *)0) return;	
	
	char ch = 0x00;
	int counter = 0;
	
	while((ch != ch1) && (ch != ch2)){
		
		ch = UART1_Receive();
		if((ch == ch1) || (ch == ch2))
			break;
		*str++ = ch;
		counter++;
	}
	return counter;
}
/*******************************************************************
* Diese Routine empfängt einen Charakter und sendet zurück an den PC
********************************************************************/
void Echo()
{
	char ch = UART1_Receive();
	
	UART1_Send_Char(ch);
	
}

/*******************************************************************
* Diese Routine schreibt in einem beliebigen Register
********************************************************************/
void writeTo(void* ptAddr, unsigned char val){
	*(unsigned char *)ptAddr = val; 
}

/*******************************************************************
* Diese Routine liest von einem beliebigen Register
********************************************************************/
unsigned char readFrom(void* ptAddr){
	return (*(unsigned char *)ptAddr); 
}


void UART1_Init(unsigned int BaudRate){							  
	unsigned int frequenzteiler = 0;						// Frequenzteiler erzeugt die Baudrate durch herunterteilen des Taktes
															// der Peripherial-Clock (P-Clock).
															// ------
															// Register PINSEL0 ist ein Spezialregister, in welchem die Funktionen der Port-Pins festgelegt werden
															// 00 - 11. In PINSEL0 koennen die Pins P0.0 - P0.15 geaendert werden							
	PINSEL0 = 0x0050000;									// Konfigurieren der Port Pins in Register PINSEL0 der Pins P0.8 (01 = TxD (UART-Sende-Pin)) und 
															// P0.9 (01 = RxD (UART-Empfangs-Pin))
															// ------																
	U1LCR = 0x83;											// Line Control Register
															// DLAB Bit wird auf 1 gesetzt, 
															// um auf die Register UxDLL und UxDLM zuzugreifen
															// ------																			
	frequenzteiler = ((PCLK)/(BaudRate*16));				// Formel zum berechnen des Frequenzteilers 							
	U1DLL = (frequenzteiler % 256) & (0xFF); 				// Low Byte des Frequenzteilers						
	U1DLM = ((frequenzteiler / 256) & (0xFF000000)) >> 24;	// High Byte des Frequenzteilers										
															// ------ 
	U1LCR = 0x03;											// Einstellung der Baudrate abgeschlossen 
															// -> entfernen des DLAB-Bit (DLAB-Bit = 0), da sonst kein Zugriff auf
															// uebrige Kontrollregister moeglich ist
															// ------									 																		
	U1FCR = 0x07;											// FCR = Register zur Steuerung der Sende- und Empfangs Fifo-Speicher.
															// 0x07 = Bit 0 = 1 -> Fifo's aktivieren
															// 		= Bit 1 = 1 -> Empfangs-FIFO ruecksetzen (loescht sich danach selbst)
															//  	= Bit 2 = 1 -> Sende-Fifo ruecksetzen	(loescht sich danach selbst)
 }
 
 char charToHex(signed char in) {
    if (in >= 0 && in <= 9) {
        return in + 0x30;
    }

    if (in >= 10 && in <= 15) {
        in -= 10;
        return in + 0x41;
    }

    return (char) -1;
}

void charToString(unsigned char c, char* dest) {
    const char LSB = (c & 0xF);
    const char MSB = (c & 0xF0) >> 4;
    dest[0] = charToHex(MSB);
    dest[1] = charToHex(LSB);
    dest[2] = 0;
}

void copy(void* dst, void* src){
	char* ptDst = (char*)dst;
	char* ptSrc = (char*)src;
	
	while((*ptSrc != 0x0D) && (*ptSrc != 0x20)){
		*ptDst++ = *ptSrc++
	
	}
}

int main()
{
	//variable für die Kommando die von PC Command
	char cmd;
	
	char receiveData[20];
	unsigned int* address;
	unsigned char value;
	int empfangeneLaenge = 0;
	
	//UART schnittstelle erstmal mit dem BaudRate 19200 initialisieren
	UART1_Init(19200);
	
	while(1){
		//Kommando wird empfangen und geprüft
		cmd = UART1_Receive()
		
		//wird für Groß und Kleinschreibung erlaubt
		if((cmd == 'D') || (cmd == 'd'))
		{
			char tmp[20];
			empfangeneLaenge = receive_String((char *)&tmp[0]);
			copy((char*)&receiveData[0], (char *)&tmp[0]);
			address = (unsigned int*)receiveData;
			
			//Liest vom register
			value = readFrom((void *)(*address));
			
			char data[empfangeneLaenge + 6];
			data[0] = 0x0D;  //CR
			data[1] = 0x0A;  //LF
			
			
			
		}else if((cmd == 'E') || (cmd == 'e'))
		{
			char tmp[20];
			empfangeneLaenge = receive_StringUntil((char *)&tmp[0], 0x20, 0x0D);
			address = (unsigned int *)&tmp[0];
			empfangeneLaenge = receive_StringUntil((char *)&tmp[0], 0x20, 0x0D);
			value = *((unsigned char *)&tmp[0]);
			
			//schreibt im register
			writeTo((void *)(*address), value);
		
		}
		
	}
	
}
