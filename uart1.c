

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
* Diese Routine sendet einen String mit definierte länge über den 
* UART shnittstelle
********************************************************************/
void UART1_SendString_Len(char* ptCh, unsigned int len)
{
	//prüfen dass der Pointer nicht NULL ist
	//falls es NULL ist, soll die Funktion sofort verlassen
	if(ptCh == (void *)0) return;
	
	while(len--){
			
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
* Diese Routine empfängt einen String bis die charackter ch1 
* gesendet ist
********************************************************************/
int receive_StringUntil(char* str, char ch1)
{
	//prüfen dass der Pointer nicht NULL ist
	//falls es NULL ist, soll die Funktion sofort verlassen
	if(str == (void *)0) return;	
	
	char ch = 0x00;
	int counter = 0;
	
	while(ch != ch1){
		
		ch = '\0';
		ch = UART1_Receive();
		if(ch == ch1)
			break;
		if(ch != 0){
		  *str++ = ch;
		  counter++;
		}
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
void writeTo(void* ptAddr, unsigned int val){
   if(ptAddr != (void *)0)
      *((unsigned int *)ptAddr) = val;
}

/*******************************************************************
* Diese Routine liest von einem beliebigen Register
********************************************************************/
unsigned int readFrom(void* ptAddr){
   if(ptAddr != (void *)0)
      return  *((unsigned int *)ptAddr);
   else
      return 0;
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

void copy(void* dst, void* src, int len){
	char* ptDst = (char*)dst;
	char* ptSrc = (char*)src;

	while(len--){
		*ptDst++ = *ptSrc++;

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
	  int isCmdOk = 0;
	  cmd = 0;
	  receivedSpaceChar = 0;
	  do{
		  cmd = UART1_Receive();
		  if((cmd == 'D') || (cmd == 'E')){
			  isCmdOk = 1;
		  }else if(cmd != '\0'){
			 //ein falsches Kommando wurde übergeben
			 //Anwender informieren
			  UART1_SendString_Len("\n\rDas Kommando ist falsch. Geben Sie D oder E ein", 49);
			  isCmdOk = 0;
			  cmd = 0;
		  }
	  	}while(!isCmdOk);

		//Leere taste wird geprüft
		do{
		  receivedSpaceChar = UART1_Receive();
		  //Leerzeichen prüfen
		  if((receivedSpaceChar != ' ') && (receivedSpaceChar != 0x00)){
			 //ein falsches Taste für leere Taste wurde übergeben
			 //Anwender informieren
			  UART1_SendString_Len("\n\rFalsche Taste gedrueckt. Druecken sie die Leertaste", 53);
			  receivedSpaceChar = 0;
		  }
		}while(receivedSpaceChar != ' ');

		//wird nur für Großschreibung erlaubt
		if(cmd == 'D')
		{
			char tmp[20] = {0};
			char specialChar;
			//8 charackter für die Adresse
			char addr[8];
			int i;
			char temp;
			char valueChar[8];

			//bis zum leerzeichen empfangen
			empfangeneLaenge = receive_StringUntil((char *)&tmp[0], 0x20);
			receive_CharUntil((char *)&specialChar, 0x0D);
			
			if(charArryToHex((char *)&tmp[0], &address)){
				//vom register lesen
				value = readFrom((void *)address);

				i = hexToCharArry(value, &valueChar);

				char data[empfangeneLaenge + 7];

				data[0] = 0x0D;  // CR
				data[1] = 0x0A;  // LF
				data[2] = 0x20;  // ' '
				copy((char*)&data[3], (char *)&tmp[0], 8);
				data[11] = ':';
				data[12] = ' ';


				UART1_SendString_Len(data, 13);
				UART1_SendString_Len(valueChar, 8);
				UART1_SendString_Len("\n\r", 2);

				//TODO Es fehlt nur noch diese Zeile auszugeben:
				//CR, LF, „4000000A: C8“


			}else{
				//Error
				//Der eingegeben Charakter ist falsch.
				//Message: Geben sie ein Zahl zwischen 0 und 9 oder Charakter (Buchstabe zwischen A und F)
			}

		}else if(cmd == 'E')
	  		{
				char specialChar;
	  			char tmp[20];
	  			char valueChar[8] = {'\0'};
	  			empfangeneLaenge = receive_StringUntil((char *)&tmp[0], 0x20);
	  			int valueLength = receive_StringUntil((char *)&valueChar[0], 0x0D);

	  			if(charArryToHex((char *)&tmp[0], &address) &&
	  					charArryToValue((char *)&valueChar[0], &value, valueLength)){
	  				//schreibt im register
	  				writeTo((void *)address, value);
	  			}else{
	  				//Invalid address
					UART1_SendString_Len("\n\r Die Adresse ist ungueltig", 28);
	  			}

	  		}
	  		else{
	  			//Error

	  		}
		
	}
	
}
