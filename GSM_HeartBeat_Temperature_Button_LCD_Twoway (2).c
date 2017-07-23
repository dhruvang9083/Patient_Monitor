#include <REGX51.H>

#define  YES    1
#define   NO    0
#define  RUN    1
#define  STOP   0
#define NOT_PRESSED 1

unsigned int char_recieved;
unsigned char char_counter,temperory,debugger;
unsigned char command_incorrect_char_received;

bit  message_started,message_ended;
bit  command_Rx_correct;
unsigned char  messege_buffer[8];
code unsigned char status[8]="*STATUS@";


#define  HEART_PULSE  P0_4	// pulse absent logic = 0, pulse present logic = 1
#define  PRESENT  1
#define  ABSENT   0


#define HEART_RATE_NORMAL_LED 	P3_2
#define HEART_RATE_ABNORMAL_LED P3_3
#define EMERGENCY_BUTTON    	P0_3

#define PRESSED 0


code unsigned char command1[10] ="at+cmgf=1 ";
code unsigned char command2[18]="at+cnmi=1,2,0,0,0 "; 
code unsigned char send_sms_command[9]="AT+CMGS= ";
code unsigned char destination1[14]="+919558880337 ";

#define ON	0
#define OFF 1


#define LCD P2
#define RS  P0_0
#define RW  P0_1
#define EN  P0_2

void lcdcmd (unsigned char value);
void lcddata (unsigned char value);
void MSDelay (unsigned int itime);
code  unsigned char line1[16]     = "Heart Beat-Temp.";
code  unsigned char line2[16]     = " Monitor By GSM ";	                
code  unsigned char line3[16]     = " Dhruvang Nirav ";
code  unsigned char college[16]   = "GIT-EC-VIII-2014";
code  unsigned char title[16]     = "Beat-Temp Meter ";
code  unsigned char emergency[16] = "Need Urgent Help";
unsigned int local_buffer,temp; 
unsigned char datatolcd[16] = "BPM=   :Temp=   ";
unsigned int duration,total,absent_timer;
unsigned int timer0_interrupt_counter,delta,loopcounter;
unsigned char average_beats_per_min,previous,beats_per_min[5];
bit finger_absent,finger_position_proper;

sbit wr = P3^5;  //START pin defined  
sbit intr=P3^6;	 //EOC pin defined	 
sbit rd = P3^7;	 // rd == output enable 
//code unsigned char decimal[10] = "0123456789";
unsigned char x,t2,t1,t0;
// end of ADC group
#define   TEMPERATURE P1
unsigned char temperature;


//unsigned char i;
void serial_receiption() interrupt 4
{
    if(RI == 1)
    { temperory = SBUF;
      if(temperory == '*')
       {
         message_started = YES;
		messege_buffer[char_counter] = SBUF;
		 char_counter++;


       }
      
      else if( message_started == YES )
        {
           messege_buffer[char_counter] = SBUF;
           char_counter++;
           
        }

       if( temperory == '@')
        {
           message_ended = YES;
           

       }

      
    }
 

}


void main(void)
{   unsigned char i,index;
      

	  HEART_RATE_NORMAL_LED = HEART_RATE_ABNORMAL_LED =  OFF; 

	  IE = 0x92; // global, Serial & timer0	int enable	1000 0010
	  IP = 0x02; // timer0 priority
	  TMOD = 0x21; // 0010 0001 timer1 in mode2 & timer0 in 16 bit mode 
	  TR1 = 1;
	  SCON =  0x40;  // 0100 0000 8bit UART 
	  TH1 = -3;
	  
	  timer0_interrupt_counter=0;
    
    lcdcmd(0x38);		// 2 line & 5*7 matrix
	MSDelay(50);
	lcdcmd(0x0C);		// display on cursor blinking
	MSDelay(50);
	lcdcmd(0x01);		// clear display screen
	MSDelay(50);

	lcdcmd(0x80);		// line 1, position 0
	MSDelay(50);

	for(i=0; i<16; i++)
	{
		lcddata(line1[i]);
		MSDelay(1);
		
	}

	lcdcmd(0xC0);		// line 1, position 0
	MSDelay(50);

	for(i=0; i<16; i++)
	{
		lcddata(line2[i]);
		MSDelay(1);
		
	}

	MSDelay(2000);

    lcdcmd(0x80);		// line 1, position 0
	MSDelay(50);

	for(i=0; i<16; i++)
	{
		lcddata(line3[i]);
		MSDelay(1);
		
	}

	   lcdcmd(0xC0);	
	MSDelay(50);

	for(i=0; i<16; i++)
	{
		lcddata(college[i]);
		MSDelay(1);
		
	}


	MSDelay(5000);

  	   
		//************* start of initialisation codes of GSM ************************************
  	     
		 SBUF = 'a'; 
		 MSDelay(10);
	   	 SBUF = 't'; 
		 MSDelay(10);

        

       MSDelay(1000);// waiting for OK signal

		  
		 MSDelay(5);


	for(i=0; i<9; i++)
	{
        SBUF = command1[i];
		MSDelay(5);
   
     } 

	for(i=0; i<17; i++)
	{
	    
		SBUF = command2[i]; 
		MSDelay(5);
	} 

        
     
	  MSDelay(1000);// waiting for OK signal
	 
//************* end of initialisation codes of GSM **************************************

   lcdcmd(0x01);		// clear display screen
	MSDelay(50);

	lcdcmd(0x80);		// line 1, position 0
	MSDelay(50);

		  
		 for(i=0; i<16; i++)
		{
			lcddata(title[i]);
			MSDelay(1);
			
		}

 while(1)
	{
   //    start of hear beat ssssssssssssssssssssssssssssssssssssssssssssssssssssssss
        
		while(HEART_PULSE == PRESENT);
		while(HEART_PULSE == ABSENT);
	
		while(HEART_PULSE == PRESENT);
		while(HEART_PULSE == ABSENT);
		
         timer0_interrupt_counter=0;
		  	TR0 = RUN; // timer0 starts	
		while(HEART_PULSE == PRESENT);
		while(HEART_PULSE == ABSENT);
		
		while(HEART_PULSE == PRESENT);
		while(HEART_PULSE == ABSENT);
		
		while(HEART_PULSE == PRESENT);
		while(HEART_PULSE == ABSENT);
		
		while(HEART_PULSE == PRESENT);
		while(HEART_PULSE == ABSENT);
		
		while(HEART_PULSE == PRESENT);
		while(HEART_PULSE == ABSENT); 
		       		   	  
		  
		  
		  TR0 = STOP;
	
		duration =  timer0_interrupt_counter * 71.11 +
						((TH0 * 256 + TL0)*1.08507)/1000;//duration of 1 beat in millisec 
	  
		 duration = (int)(duration/5.0);
		average_beats_per_min = (((60 * 1000)/(duration)));
	
		datatolcd[4] = (average_beats_per_min / 100) + '0';
		temp = (average_beats_per_min % 100);
		datatolcd[5] = temp/10 + '0';
		datatolcd[6] = temp%10 + '0'; 

		
//  *********************Temperature monitoring codes***********************
	  
	 	//Code for ADC input
		rd = 1;
		wr = 1;
		wr=0;
		wr = 1;		
		while(intr==0);
	// Data converted into Digital form 
	   	 rd = 0;
		 x = TEMPERATURE;
		 temperature = TEMPERATURE;	

		 datatolcd[13] = (temperature / 100) + '0';
		temp = (temperature  % 100);
		datatolcd[14] = temp/10 + '0';
		datatolcd[15] = temp%10 + '0';

	
		lcdcmd(0xC0);
		MSDelay(10);

		
  	for(i = 0; i< 16; i++)
		{
		 lcddata(datatolcd[i]);
			 MSDelay(1);
		}
			 
			 
//   transmission
		if((average_beats_per_min<60) || (average_beats_per_min>95) || (temperature > 45))
   		{
			 
			HEART_RATE_NORMAL_LED = OFF;
			HEART_RATE_ABNORMAL_LED = ON;
//			BUZZER = ON;
			for(i=0; i<8; i++)
			{
			     
				SBUF = send_sms_command[i];
				MSDelay(5); 
				
		      
			
		    }
		         
			for(i=0; i<13; i++)
			{
		        
				SBUF = destination1[i]; 
				MSDelay(5);
			
		    }
		
					
		     for(i=0; i<16; i++)
			{ 	 
				SBUF = datatolcd[i]; 
				MSDelay(5);
			   }
		
		}

		else
		{
			HEART_RATE_NORMAL_LED = ON;
			HEART_RATE_ABNORMAL_LED = OFF;
//			BUZZER = OFF;
		}

		if(EMERGENCY_BUTTON == PRESSED)
		{
			
			lcdcmd(0xC0);
			MSDelay(10);
			
			for(i = 0; i< 16; i++)
			{
		 		lcddata(emergency[i]);
			 	MSDelay(1);
			}
			 
			HEART_RATE_NORMAL_LED = OFF;
			HEART_RATE_ABNORMAL_LED = ON;
//			BUZZER = ON;
			for(i=0; i<8; i++)
			{
			     
				SBUF = send_sms_command[i];
				 	MSDelay(5); 
				
		      
			
		    }
		        
			
		      
			for(i=0; i<13; i++)
			{
		        
				SBUF = destination1[i]; 
				MSDelay(5);
				
			
		    }
		
				
		     for(i=0; i<16; i++)
			{ 	 
				SBUF =emergency[i]; 
					MSDelay(5);
			   }
		
			
		
			  MSDelay(5000);
			
		
		}

//*********************************************************************************************
if( message_ended == YES )
    {  
	 	command_incorrect_char_received = 0; 
    	for(index = 0; index < 8; index++)
  		{
	      if((status[index] != messege_buffer[index]))
          	command_incorrect_char_received++;
     	}

	 if(command_incorrect_char_received == 0)
	 {
	 		 
			HEART_RATE_NORMAL_LED = ON;
			HEART_RATE_ABNORMAL_LED = ON;
			MSDelay(1000);
			HEART_RATE_NORMAL_LED = OFF;
			HEART_RATE_ABNORMAL_LED = OFF;
			MSDelay(1000);
			for(i=0; i<8; i++)
			{
			     
				SBUF = send_sms_command[i];
				MSDelay(5); 
			 
			
		    }
		        
			
		      
			for(i=0; i<13; i++)
			{
		        
				SBUF = destination1[i]; 
				MSDelay(5);
			
		    }
		
		  
				
		     for(i=0; i<16; i++)
			{ 	 
				SBUF = datatolcd[i]; 
					MSDelay(5);
			   }
		
			
		

			HEART_RATE_NORMAL_LED = ON;
			HEART_RATE_ABNORMAL_LED = ON;
			MSDelay(1000);
			HEART_RATE_NORMAL_LED = OFF;
			HEART_RATE_ABNORMAL_LED = OFF;
			MSDelay(1000);
		    
			  MSDelay(5000);
				
	 }

	  
   }
//*******************************************************************************************

	}  //  end of super while

}

void lcdcmd (unsigned char value)
{
	LCD = value;		// put value on the pins
	RS = 0;
	RW = 0;
	EN = 1;				// strobe the enable pin
	MSDelay(1);
	EN = 0;
	return;
}

void lcddata (unsigned char value)
{
	LCD = value;		// put the value on the pins
	RS = 1; 
	RW = 0;
	EN = 1;				// strobe the enable pin
	MSDelay(1);
	EN = 0;
	return;
}

void MSDelay (unsigned int itime)
{
	unsigned char i,j;
	for(i=0; i<itime; i++)
	{
		for(j=0; j<100; j++)
		{;}
	}
}

void timer0() interrupt 1  // timer0 runs from 0x0000 to 0xFFFF  
{
	timer0_interrupt_counter++;
}


  