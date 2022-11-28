#ifndef GPIO_H_
#define GPIO_H_

/*
AVR Registers ------------------------
DDR : this register used to set Direction of Port (Input/Output) like : [DDRA,DDRB,DDRC,DDRD] so set 1 mean output , set 0 mean input ,every pin take this example [DDA0:DDA7]
GPIO_PORT: this register used to write low/high for pins set 1 for high , set 0 for low like [PORTA,PORTB,PORTC,PORTD] ,every pin take this example [PORTA0:PORTA7]
GPIO_PIN : this register used to read pin Input status where 0 mean low , 1 mean high like [PINA,PINB,PINC,PIND] , every pin take this example [PINA0:PINA7]
*/

////////////////////////////////////////////
//ENUMS
////////////////////////////////////////////

#pragma region ENUMS

//Define The Port ID
typedef enum
{
	GPIO_PORT_A,
	GPIO_PORT_B,
	GPIO_PORT_C,
	GPIO_PORT_D
}GPIO_PORT;

//Define The Pin ID
typedef enum
{
	GPIO_PIN_0=0,
	GPIO_PIN_1=1,
	GPIO_PIN_2=2,
	GPIO_PIN_3=3,
	GPIO_PIN_4=4,
	GPIO_PIN_5=5,
	GPIO_PIN_6=6,
	GPIO_PIN_7=7
}GPIO_PIN;

//Define the Port_Pin
typedef enum
{
	GPIO_IO_PA0,
	GPIO_IO_PA1,
	GPIO_IO_PA2,
	GPIO_IO_PA3,
	GPIO_IO_PA4,
	GPIO_IO_PA5,
	GPIO_IO_PA6,
	GPIO_IO_PA7,
	
	GPIO_IO_PB0,
	GPIO_IO_PB1,
	GPIO_IO_PB2,
	GPIO_IO_PB3,
	GPIO_IO_PB4,
	GPIO_IO_PB5,
	GPIO_IO_PB6,
	GPIO_IO_PB7,
	
	GPIO_IO_PC0,
	GPIO_IO_PC1,
	GPIO_IO_PC2,
	GPIO_IO_PC3,
	GPIO_IO_PC4,
	GPIO_IO_PC5,
	GPIO_IO_PC6,
	GPIO_IO_PC7,
	
	GPIO_IO_PD0,
	GPIO_IO_PD1,
	GPIO_IO_PD2,
	GPIO_IO_PD3,
	GPIO_IO_PD4,
	GPIO_IO_PD5,
	GPIO_IO_PD6,
	GPIO_IO_PD7
	
}GPIO_IO;


//Define the Data Direction
typedef enum
{
	GPIO_DIRECTION_INPUT,
	GPIO_DIRECTION_OUTPUT,
	GPIO_DIRECTION_PULLUP
}GPIO_DIRECTION;

//Define the Data Status
typedef enum
{
	GPIO_VALUE_LOW,
	GPIO_VALUE_HIGH,
	GPIO_VALUE_UnKnown,
	GPIO_VALUE_TOGGLE
}GPIO_VALUE;


#pragma endregion ENUMS

////////////////////////////////////////////
//BitsWise Operations
////////////////////////////////////////////

#pragma region BitWise Operations


//Bits Operations ---------------

//Set 1 inside Bit For Register
#define BITWISE_SET_BIT(register,bitNumber) register |= (1<<bitNumber)

//Set 0 inside Bit For Register
#define BITWISE_CLEAR_BIT(register,bitNumber) register &= ~(1<<bitNumber)

//Toggle Bit Value For Register
#define BITWISE_TOGGLE_BIT(register,bitNumber) register ^= (1<<bitNumber)

//Read Bit Value 1 or 0
#define BITWISE_GET_BIT(register,bitNumber) ((register>>bitNumber) & 0x01)

//Read High Byte Value From 16Bits
#define BITWISE_GET_HIGH_BYTE(register_16_Bits) (uint8_t)(register_16_Bits>>8)

//Read LOW Byte Value From 16Bits
#define BITWISE_GET_LOW_BYTE(register_16_Bits) (uint8)(register_16_Bits & 0x00FF)

//Read Byte Value From 8Bits
#define BITWISE_GET_BYTE(register_8_Bits) (uint8)(register_8_Bits)

//Registers Operations --------------

//Set 1 inside Port For Register
#define BITWISE_SET_REGISTER(register) register = 0xff

//Set 0 inside Port For Register
#define BITWISE_CLEAR_REGISTER(register) register = 0x00

//Toggle Bit Value For Register
#define BITWISE_TOGGLE_REGISTER(register) register ^= 0xff


#pragma endregion BitWise Operations


class GPIO
{
	
	public:
	
	////////////////////////////////////////////
	//Port Operations
	////////////////////////////////////////////

	//Set Direction Of Port INPUT/OUTPUT
	static void Port_Direction(GPIO_PORT port,GPIO_DIRECTION direction);

	//Write Value over Port pins
	static void Port_Write(GPIO_PORT port,unsigned char value);

	//Write High/LOW/Toggle Over Port pins
	static void Port_Write(GPIO_PORT port,GPIO_VALUE value);

	//Read Port Value (Byte)
	static char Port_Read(GPIO_PORT port);

	

	////////////////////////////////////////////
	//Pin Operations
	////////////////////////////////////////////


	//Set Direction Of Pin INPUT/OUTPUT
	static void Pin_Direction(GPIO_PORT port,GPIO_PIN pin,GPIO_DIRECTION direction);

	//Set Direction Of Pin INPUT/OUTPUT
	static void Pin_Direction(GPIO_IO pin,GPIO_DIRECTION direction);

	//Write High/LOW/Toggle Over pin
	static void Pin_Write(GPIO_PORT port,GPIO_PIN pin,GPIO_VALUE value);
	
	//Write High/LOW/Toggle Over pin
	static void Pin_Write(GPIO_IO pin,GPIO_VALUE value);
	
	
	
	//Write High Over pin
	static void Pin_Set(GPIO_PORT port,GPIO_PIN pin);
	
	//Write High Over pin
	static void Pin_Set(GPIO_IO pin);


	//Write LOW Over pin
	static void Pin_Clear(GPIO_PORT port,GPIO_PIN pin);

	//Write LOW Over pin
	static void Pin_Clear(GPIO_IO pin);


	//Write Toggle Over pin
	static void Pin_Toggle(GPIO_PORT port,GPIO_PIN pin);

	//Write Toggle Over pin
	static void Pin_Toggle(GPIO_IO pin);
	
	
	
	//Read Pin Value (HIGH/LOW)
	static GPIO_VALUE Pin_Read(GPIO_PORT port,GPIO_PIN pin);
	
	//Read Pin Value (HIGH/LOW)
	static GPIO_VALUE Pin_Read(GPIO_IO pin);

};


#endif /* GPIO_H_ */