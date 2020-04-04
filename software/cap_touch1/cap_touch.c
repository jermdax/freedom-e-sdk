#include <stdio.h>
#include <assert.h>
#include <metal/gpio.h>

enum gpioPin{
  GPIO0 = 0,
  GPIO1 = 1,
  GPIO2 = 2,
  GPIO3 = 3,
  GPIO4 = 4,
  GPIO5 = 5,
  //skip
  GPIO9 = 9,
  GPIO10 = 10,
  GPIO11 = 11,
  GPIO12 = 12,
  GPIO13 = 13,
  //skip
  GPIO16 = 16,
  GPIO17 = 17,
  GPIO18 = 18,
  GPIO19 = 19,
  GPIO20 = 20,
  GPIO21 = 21,
  GPIO22 = 22,
  GPIO23 = 23,
};

enum pinVal {
  HIGH,
  LOW
};

static void setPinInput(struct metal_gpio *metal_gpio_dev, enum gpioPin pinId) {
  if(metal_gpio_enable_input(metal_gpio_dev, pinId) != 0) {
    printf("Failed to set pin %d as input \n", pinId);
  }
}

static void setPinOutput(struct metal_gpio *metal_gpio_dev, enum gpioPin pinId) {
  if(metal_gpio_enable_output(metal_gpio_dev, pinId) != 0) {
    printf("Failed to set pin %d as output \n", pinId);
  }
}

static void setOutputVal(struct metal_gpio *metal_gpio_dev, enum gpioPin pinId, enum pinVal val) {
    if(metal_gpio_set_pin(metal_gpio_dev, pinId, val) != 0) {
      printf("Failed to set pin: %d to value: %d\n", pinId, val);
    }
}

int main(int argc, char *argv[]) {
  enum pinVal writeval = LOW;
  struct metal_gpio* gpio_dev = metal_gpio_get_device(0);
  setPinOutput(gpio_dev, GPIO2);
  while(1) {
    printf("Loop start\n");
    unsigned int i = 0;
    while(++i < 0xfffff);
    i = 0;
    writeval = writeval == LOW ? HIGH : LOW;
    setOutputVal(gpio_dev, GPIO2, writeval);
    printf("One loop\n");
  }
  printf("done\n");
  return 0;
}
