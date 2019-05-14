#include <stdlib.h>
#include <bcm2835.h>

#define OUT_OF_RANGE_PIN_NUMBER 99

struct ad7091r_thing {
  int convst_pin;
  int cs_pin;
  int clk_pin;
  int data_pin;
};

typedef struct ad7091r_thing ad7091r_thing;

ad7091r_thing * ad7091r_alloc_instance(void)
{
  ad7091r_thing *thing;
  
  thing = malloc(sizeof(ad7091r_thing));

  if(thing != 0) 
  {
    thing->convst_pin = OUT_OF_RANGE_PIN_NUMBER;
    thing->cs_pin = OUT_OF_RANGE_PIN_NUMBER;
    thing->clk_pin = OUT_OF_RANGE_PIN_NUMBER;
    thing->data_pin = OUT_OF_RANGE_PIN_NUMBER;
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

int ad7091r_cs_pin(ad7091r_thing *pthing, int n_cs_pin)
{
  if(pthing != 0) 
  {
    pthing->cs_pin = n_cs_pin;
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


int ad7091r_begin(ad7091r_thing *pthing)
{
  if(pthing != 0)
  {
    if(pthing->convst_pin == OUT_OF_RANGE_PIN_NUMBER ||
       pthing->cs_pin == OUT_OF_RANGE_PIN_NUMBER ||
       pthing->clk_pin == OUT_OF_RANGE_PIN_NUMBER ||
       pthing->data_pin == OUT_OF_RANGE_PIN_NUMBER)
       {
          return 0;
       }

    if(!bcm2835_init())
    {
      return 0;
    }
    
    bcm2835_gpio_fsel(pthing->convst_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(pthing->cs_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(pthing->clk_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(pthing->data_pin, BCM2835_GPIO_FSEL_INPT);

 
    bcm2835_gpio_write(pthing->cs_pin, HIGH);
    bcm2835_gpio_write(pthing->clk_pin, LOW);
    bcm2835_gpio_write(pthing->convst_pin, HIGH);
  
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

    bcm2835_gpio_write(pthing->cs_pin, LOW);        //Set CS Pin to LOW
    delayMicroseconds(1);

    uint8_t clock_cycle = 0;

    for(clock_cycle = 0; clock_cycle < 4; clock_cycle++) //4 cycles only to short the cycle; you can choose between 2 to 8
    { 
      bcm2835_gpio_write(pthing->clk_pin, HIGH); //Set CLK Pin to HIGH
      delayMicroseconds(1);
      bcm2835_gpio_write(pthing->clk_pin, LOW); //Set CLK Pin to LOW
      delayMicroseconds(1);
    }

    bcm2835_gpio_write(pthing->cs_pin, HIGH);

    //Perform Another conversion
    bcm2835_gpio_write(pthing->convst_pin, LOW); //Set CONVST Pin to LOW
    delayMicroseconds(1);
    bcm2835_gpio_write(pthing->convst_pin, HIGH);  //Set CONVST Pin to HIGH

    for(clock_cycle = 0; clock_cycle < 12; clock_cycle++) 
    { 
      bcm2835_gpio_write(pthing->clk_pin, HIGH); //Set CLK Pin to HIGH
      delayMicroseconds(1);
      bcm2835_gpio_write(pthing->clk_pin, LOW); //Set CLK Pin to LOW
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

    //Start a conversion
    bcm2835_gpio_write(pthing->convst_pin, LOW); //Set CONVST Pin to LOW
    delayMicroseconds(1);
    bcm2835_gpio_write(pthing->convst_pin, HIGH);  //Set CONVST Pin to HIGH
  
    bcm2835_gpio_write(pthing->cs_pin, LOW);        //Set CS Pin to LOW
  
    uint8_t clock_cycle = 0;
    for(clock_cycle = 0; clock_cycle < 12; clock_cycle++) { 
      bcm2835_gpio_write(pthing->clk_pin, HIGH); //Set CLK Pin to HIGH
      delayMicroseconds(1);
      bcm2835_gpio_write(pthing->clk_pin, LOW); //Set CLK Pin to LOW
      result = (result << 1) | (bcm2835_gpio_lev(pthing->data_pin) & 0x01);
      delayMicroseconds(1);
    }
  
    delayMicroseconds(1);
    bcm2835_gpio_write(pthing->cs_pin, HIGH);

    return result;
  }
  else {
    return 0;
  }

}