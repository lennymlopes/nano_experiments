#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string>
#include <iostream>

#define PIN GPIO_NUM_4
using namespace std;

// see https://www.learnesp32.com/3_blinkey for details
void blinky(void *params)
{
	gpio_pad_select_gpio(PIN);
	gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
	int isOn = 0;
	while (true)
	{
		isOn = !isOn;
		gpio_set_level(PIN, isOn);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
class Person
{
  int _age;
  string _name;

public:
  Person(string, int);
  void SayHello()
  {
    cout << "Hi I'm " << this->_name << " and I'm " << this->_age << " years old\n";
  }
};

Person::Person(string name, int age)
{
  _name = name;
  _age = age;
}
extern "C" void app_main(void)
{
  printf("Hello world!\n");
  
  xTaskCreate(&blinky, "blink led", 2048, NULL, 2, NULL);
   Person john("John", 23);
   john.SayHello();
}
