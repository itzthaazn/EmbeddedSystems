		 /*
		  * 
		  *
		  * Created: 16/08/2015 12:06:56 p.m.
		  *  Author: wgb8191
		  */ 
		 
		 
		 #define F_CPU 8000000UL
		 #include <avr/io.h>

		 #define Temperture ADCW 				//Sets the ADC value as Temperture
		 #define heaterSwitch PINA & (1<<0) 	//Set pin0 as heater switch
		 #define HeaterOn (PORTB |= (1<<7))		//Turns Heaters on for testing
		 #define HeaterOff (PORTB &= ~(1<<7))	//Turns Heaters on for testing
		 #define display PORTC					//Define display as output for degrees
		 #define char hex;
		 #define char ten;
		 #define char ones;
		 
		 
		 
		 
		 char TWIwrite(char address, char ref_addr , char data); //Calls the TWI for light colours
		 void setup(void);
		 
		 
		 int main(void)
		 {
		 	setup();
		 
		     while(1)
		     {	
		 
//			Calculations:
// 		  	5v/1024(Revolutions)
// 		  	50mv / 5mv = 10

		 		//Converting values
		 		hex  = Temperture/10; 
		 		ten = hex/10;
		 		ones = hex%10;
		 		result = (ten<<4)+(ones);
		 
		 		display = result; // Displays temperture on display
				 
		 		
		 		if(heaterSwitch)//Turns Heater On when switch is on
		  		{
		 			HeaterOn;
		 		}
		 		else 
		 			HeaterOff;
		 			
		 	
		 		if(hex <= 35){ // Bellow 36
		 		TWIwrite(0b11001100, 0b00000011, 0b00110000); //Blue light turns on when it's bellow 36 degree
		 		}
		 			
		 		if(hex == 36 || hex == 37 ||hex = 38) {  //37-38
		 		TWIwrite(0b11001100, 0b00000011, 0b00000011); //Green light turns on when it's 36-38 degrees
		 		}
		 		
		 		if(hex >= 39){//39degree+
		 			TWIwrite(0b11001100, 0b00000011, 0b00001100);//Red light turns on when its above 38 degrees
		 		}
		 	}
		 	
		 	
		 	
		 }
		 
		 char TWIwrite(char address, char reg_addr, char data)
		 {
		 	char x, error = 0;
		 
		 	TWCR = (1<<TWINT)|(1<<TWSTA |(1<<TWEN));
		 	while(!(TWCR & (1<<TWINT)));
		 	if((TWSR & 0xF8)!= 0x08)
		 		error= 1;
		 	else{
		 		TWDR = address;
		 		TWCR = (1<<TWINT)| (1<<TWEN);
		 		while(!(TWCR & (1<<TWINT)));
		 		if((TWSR & 0xF8)!= 0x18)
		 			error = 1;
		 		else
		 		{
		 			TWDR = reg_addr;
		 			TWCR = (1<<TWINT)| (1<<TWEN);
		 			while(!(TWCR & (1<<TWINT)));
		 			if((TWSR & 0xF8)!= 0x28)
		 				error = 1;
		 			else
		 			{
		 				TWDR = data;
		 				TWCR = (1<<TWINT)| (1<<TWEN);
		 				while(!(TWCR & (1<<TWINT)));
		 				
		 				if((TWSR & 0xF8)!= 0x28)
		 					error = 1;
		 				else
		 				{
		 					TWCR = (1<<TWINT)| (1<< TWEN)|(1<<TWSTO);
		 					for (x=0; x<50; x++)
		 					asm("NOP");
		 				}
		 			}
		 		}
		 	}
		 	return(error);
		 }
		 
		 
		 	
		 	void setup(void)
		 	{
			DDRA = 0x00;// Inputs
		 	DDRB = 0xFF;// Setup for Heater
		 	DDRE = 0x03;// Using switches for pin
		 	DDRC = 0xFF;// Setup for Display
		 
		 	ADCSRA = 0b11100111;// free running adc
		 	ADMUX =  0b01000011;// left adjusted ADC3 for variable resistor 1
		 
		 	TWBR = 0x20; //How fast it travels default values
		 	TWIwrite(0b11001100, 0b00000000, 0b00111111); //turns all Blue to maximum
		 	TWIwrite(0b11001100, 0b00000001, 0b00111111); //turns all Red to maximum
		 	TWIwrite(0b11001100, 0b00000010, 0b00111111); //turns all Green to maximum
		 	}

