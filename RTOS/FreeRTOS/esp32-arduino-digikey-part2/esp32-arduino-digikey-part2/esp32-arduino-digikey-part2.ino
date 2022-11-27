#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0; //uzywanie jednego core'a do nauki latwiejsze
#else
static const BaseType_t app_cpu = 1;
#endif

// Pins
static const int led_pin = LED_BUILTIN; //jakis arduino wbudowany pin

// Our task: blink an LED at one rate
void toggleLED(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
     //non blocking function - wykonuj inne zadania do momentu az zdefiniowany czas tutaj minie tickTimer
    vTaskDelay(500 / portTICK_PERIOD_MS); //tick timer connected with interupt
   
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS); //it is one
  }
}

void setup() {
  

  //Create pin
  pinMode(led_pin,OUTPUT);

  //task to run forever one one core
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED,  // Function to be called
              "Toggle LED",   // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

  // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
  // main after setting up your tasks.
}

void loop() {
  // put your main code here, to run repeatedly:

}
