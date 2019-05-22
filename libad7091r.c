#include <stdlib.h>
#include <bcm2835.h>

#define OUT_OF_RANGE_PIN_NUMBER 99

#define READ_INDICATOR 26

//#define FOURINPUTMODE //Uses MC74HC153A and SN74HC139

struct ad7091r_thing {
  int convst_pin;                 //CONVST
  int clk_pin;                    //SCK
  int data_pin;                   //MISO
  int mosi_pin;                   //MOSI
  int ss_1_pin;                   //SS1

  int address_0_pin;              //ADDRESS0
  int address_1_pin;              //ADDRESS1
  
  int chip_address;               //0-3

  #ifdef FOURINPUTMODE
    int ss_2_pin;                 //SS2
    int data_select_pin;          //BO
    int ss_address;               //0-3
    int data_select_address;      //0-1
  #endif
};

typedef struct ad7091r_thing ad7091r_thing;

ad7091r_thing * ad7091r_alloc_instance(void)
{
  ad7091r_thing *thing;
  
  thing = malloc(sizeof(ad7091r_thing));

  if(thing != 0) 
  {
    thing->convst_pin = OUT_OF_RANGE_PIN_NUMBER;
    thing->clk_pin = OUT_OF_RANGE_PIN_NUMBER;
    thing->data_pin = OUT_OF_RANGE_PIN_NUMBER;
    thing->mosi_pin = OUT_OF_RANGE_PIN_NUMBER;
    thing->ss_1_pin = OUT_OF_RANGE_PIN_NUMBER;
    
    thing->address_0_pin = OUT_OF_RANGE_PIN_NUMBER;
    thing->address_1_pin = OUT_OF_RANGE_PIN_NUMBER;
    
    #ifdef FOURINPUTMODE
      thing->ss_2_pin = OUT_OF_RANGE_PIN_NUMBER;
      thing->data_select_pin = OUT_OF_RANGE_PIN_NUMBER;
      thing->ss_address = 0;
      thing->data_select_address = 0;
    #endif
    
  }

  return thing;
}

int ad7091r_dealloc_instance(ad7091r_thing *pthing)
{
  if(pthing != 0)
  {
    free(pthing);
    return 1;
  }
  else
  {
    return 0;
  }
}

int ad7091r_convst_pin(ad7091r_thing *pthing, int n_convst_pin)
{
  if(pthing != 0) 
  {
    pthing->convst_pin = n_convst_pin;
    return 1;
  }
  else
  {
    return 0;
  }
}

int ad7091r_clk_pin(ad7091r_thing *pthing, int n_clk_pin)
{
  if(pthing != 0) 
  {
    pthing->clk_pin = n_clk_pin;
    return 1;
  }
  else
  {
    return 0;
  }
}

int ad7091r_data_pin(ad7091r_thing *pthing, int n_data_pin)
{
  if(pthing != 0) 
  {
    pthing->data_pin = n_data_pin;
    return 1;
  }
  else
  {
    return 0;
  }
}

int ad7091r_mosi_pin(ad7091r_thing *pthing, int n_mosi_pin)
{
  if(pthing != 0) 
  {
    pthing->mosi_pin = n_mosi_pin;
    return 1;
  }
  else
  {
    return 0;
  }
}

int ad7091r_ss_1_pin(ad7091r_thing *pthing, int n_ss_1_pin)
{
  if(pthing != 0) 
  {
    pthing->ss_1_pin = n_ss_1_pin;
    return 1;
  }
  else
  {
    return 0;
  }
}

#ifdef FOURINPUTMODE
int ad7091r_ss_2_pin(ad7091r_thing *pthing, int n_ss_2_pin)
{
  if(pthing != 0) 
  {
    pthing->ss_2_pin = n_ss_2_pin;
    return 1;
  }
  else
  {
    return 0;
  }
}
#endif


int ad7091r_address_0_pin(ad7091r_thing *pthing, int n_address_0_pin)
{
  if(pthing != 0) 
  {
    pthing->address_0_pin = n_address_0_pin;
    return 1;
  }
  else
  {
    return 0;
  }
}

int ad7091r_address_1_pin(ad7091r_thing *pthing, int n_address_1_pin)
{
  if(pthing != 0) 
  {
    pthing->address_1_pin = n_address_1_pin;
    return 1;
  }
  else
  {
    return 0;
  }
}

#ifdef FOURINPUTMODE
int ad7091r_data_select_pin(ad7091r_thing *pthing, int n_data_select_pin)
{
  if(pthing != 0) 
  {
    pthing->data_select_pin = n_data_select_pin;
    return 1;
  }
  else
  {
    return 0;
  }
}
#endif

#ifdef FOURINPUTMODE
int ad7091r_ss_address(ad7091r_thing *pthing, int n_ss_address)
{
  int result = 0;
  if(pthing != 0) 
  {
    if(n_ss_address >= 0 && n_ss_address <= 3)
    {
      pthing->ss_address = n_ss_address;
      result = 1;
    }
    else
    {
      result = 0;
    }
    
  }
  else
  {
    result = 0;
  }
  
  return result;
}
#endif


int ad7091r_chip_address(ad7091r_thing *pthing, int n_chip_address)
{
  int result = 0;
  if(pthing != 0) 
  {
    if(n_chip_address >= 0 && n_chip_address <= 3)
    {
      pthing->chip_address = n_chip_address;
      result = 1;
    }
    else
    {
      result = 0;
    }
    
  }
  else
  {
    result = 0;
  }
  
  return result;
}

#ifdef FOURINPUTMODE
int ad7091r_data_select_address(ad7091r_thing *pthing, int n_data_select_address)
{
  int result = 0;
  if(pthing != 0) 
  {
    if(n_data_select_address >= 0 && n_data_select_address <= 1)
    {
      pthing->data_select_address = n_data_select_address;
      result = 1;
    }
    else
    {
      result = 0;
    }
    
  }
  else
  {
    result = 0;
  }
  
  return result;
}
#endif
  
int ad7091r_select_chip(ad7091r_thing *pthing)
{
  if(pthing != 0)
  {
    #ifdef FOURINPUTMODE
      bcm2835_gpio_write(pthing->ss_1_pin, (pthing->ss_address & 0x01) ^ 0x01);        //Need to reverse the value
      bcm2835_gpio_write(pthing->ss_2_pin, ((pthing->ss_address >> 1) & 0x01) ^ 0x01); //Need to reverse the value
    #else
      bcm2835_gpio_write(pthing->ss_1_pin, HIGH);
    #endif
    
    bcm2835_gpio_write(pthing->address_0_pin, (pthing->chip_address & 0x01));
    bcm2835_gpio_write(pthing->address_1_pin, ((pthing->chip_address >> 1) & 0x01));
    
    return 1;
  }
  else
  {
    return 0;
  }
}

int ad7091r_deselect_chip(ad7091r_thing *pthing)
{
  if(pthing != 0)
  {
    #ifdef FOURINPUTMODE
      bcm2835_gpio_write(pthing->ss_1_pin, HIGH);        
      bcm2835_gpio_write(pthing->ss_2_pin, HIGH); 
    #else
      bcm2835_gpio_write(pthing->ss_1_pin, LOW);
    #endif
    
    bcm2835_gpio_write(pthing->address_0_pin, LOW);
    bcm2835_gpio_write(pthing->address_1_pin, LOW);
    
    return 1;
  }
  else
  {
    return 0;
  }
}

int ad7091r_begin(ad7091r_thing *pthing)
{
  
  if(pthing != 0)
  {
    if(pthing->convst_pin == OUT_OF_RANGE_PIN_NUMBER         ||
       pthing->clk_pin == OUT_OF_RANGE_PIN_NUMBER            ||
       pthing->data_pin == OUT_OF_RANGE_PIN_NUMBER           ||
       pthing->mosi_pin == OUT_OF_RANGE_PIN_NUMBER           ||
       pthing->ss_1_pin == OUT_OF_RANGE_PIN_NUMBER           ||
       pthing->address_0_pin == OUT_OF_RANGE_PIN_NUMBER      ||
       pthing->address_1_pin == OUT_OF_RANGE_PIN_NUMBER 
       #ifdef FOURINPUTMODE
       || pthing->ss_2_pin == OUT_OF_RANGE_PIN_NUMBER
       || pthing->data_select_pin == OUT_OF_RANGE_PIN_NUMBER
       #endif
       )
       
       {
          return 0;
       }

    if(!bcm2835_init())
    {
      return 0;
    }
    

    bcm2835_gpio_fsel(pthing->convst_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(pthing->clk_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(pthing->data_pin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(pthing->mosi_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(pthing->ss_1_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(pthing->address_0_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(pthing->address_1_pin, BCM2835_GPIO_FSEL_OUTP);
    #ifdef FOURINPUTMODE
      bcm2835_gpio_fsel(pthing->ss_2_pin, BCM2835_GPIO_FSEL_OUTP);
      bcm2835_gpio_fsel(pthing->data_select_pin, BCM2835_GPIO_FSEL_OUTP);
    #endif
    
    bcm2835_gpio_write(pthing->address_0_pin, LOW);
    bcm2835_gpio_write(pthing->address_1_pin, LOW);
    bcm2835_gpio_write(pthing->clk_pin, HIGH);
    bcm2835_gpio_write(pthing->convst_pin, HIGH);
    
    #ifdef FOURINPUTMODE
      bcm2835_gpio_write(pthing->ss_1_pin, HIGH);
      bcm2835_gpio_write(pthing->ss_2_pin, HIGH);
      bcm2835_gpio_write(pthing->data_select_pin, LOW);
    #else
      bcm2835_gpio_write(pthing->ss_1_pin, LOW);
    #endif
    
    bcm2835_gpio_fsel(READ_INDICATOR, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(READ_INDICATOR, LOW);

    return 1;
  } 
  else
  {
    return 0;
  }
}

int ad7091r_reset(ad7091r_thing *pthing)
{
  if(pthing != 0)
  {
    //Start a conversion
    delayMicroseconds(1);
    bcm2835_gpio_write(pthing->convst_pin, LOW); //Set CONVST Pin to LOW
    delayMicroseconds(1);
    bcm2835_gpio_write(pthing->convst_pin, HIGH);  //Set CONVST Pin to HIGH

    //Selecting Chip
    ad7091r_select_chip(pthing);
      
    delayMicroseconds(1);

    uint8_t clock_cycle = 0;

    for(clock_cycle = 0; clock_cycle < 4; clock_cycle++) //4 cycles only to short the cycle; you can choose between 2 to 8
    { 
      bcm2835_gpio_write(pthing->clk_pin, LOW); //Set CLK Pin to LOW
      delayMicroseconds(1);
      bcm2835_gpio_write(pthing->clk_pin, HIGH); //Set CLK Pin to HIGH
      delayMicroseconds(1);
    }

    //Deselecting the chip
    ad7091r_deselect_chip(pthing);

    //Perform Another conversion
    bcm2835_gpio_write(pthing->convst_pin, LOW); //Set CONVST Pin to LOW
    delayMicroseconds(1);
    bcm2835_gpio_write(pthing->convst_pin, HIGH);  //Set CONVST Pin to HIGH

    for(clock_cycle = 0; clock_cycle < 12; clock_cycle++) 
    { 
      bcm2835_gpio_write(pthing->clk_pin, LOW); //Set CLK Pin to LOW
      delayMicroseconds(1);
      bcm2835_gpio_write(pthing->clk_pin, HIGH); //Set CLK Pin to HIGH
      delayMicroseconds(1);
    }

    return 1;
  }
  else
  {
    return 0;
  }

}

int ad7091r_data(ad7091r_thing *pthing)
{
  if(pthing != 0)
  {
    int result = 0;
    
    
    delayMicroseconds(100);
    //Start a conversion
    bcm2835_gpio_write(pthing->convst_pin, LOW); //Set CONVST Pin to LOW
    delayMicroseconds(1);
    
  
    ad7091r_select_chip(pthing);
    
    #ifdef FOURINPUTMODE
      bcm2835_gpio_write(pthing->data_select_pin, (data_select_address & 0x01));
    #endif
    
    uint8_t clock_cycle = 0;
    for(clock_cycle = 0; clock_cycle < 12; clock_cycle++) { 

      bcm2835_gpio_write(pthing->clk_pin, LOW); //Set CLK Pin to LOW

      
      bcm2835_gpio_write(READ_INDICATOR, HIGH);
      result = (result << 1) | (bcm2835_gpio_lev(pthing->data_pin) & 0x01);
      bcm2835_gpio_write(READ_INDICATOR, LOW);

      
      delayMicroseconds(1);
      bcm2835_gpio_write(pthing->clk_pin, HIGH); //Set CLK Pin to HIGH
      //result = (result << 1) | (bcm2835_gpio_lev(pthing->data_pin) & 0x01);
      delayMicroseconds(1);
    }
  
    bcm2835_gpio_write(pthing->clk_pin, LOW); //Set CLK Pin to LOW
    delayMicroseconds(1);
    bcm2835_gpio_write(pthing->clk_pin, HIGH); //Set CLK Pin to HIGH
    
    ad7091r_deselect_chip(pthing);
    
    delayMicroseconds(100);
    bcm2835_gpio_write(pthing->convst_pin, HIGH);  //Set CONVST Pin to HIGH
    
    //It will enter power-low mode
    return result;
  }
  else {
    return 0;
  }

}
