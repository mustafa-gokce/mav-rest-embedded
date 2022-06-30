#include <Arduino.h>

// define tasks
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

void task_1_code(void *pvParameters);
void task_2_code(void *pvParameters);

// setup function
void setup()
{

  // start serial port
  Serial.begin(9600);

  // create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
      task_1_code, /* Task function. */
      "task_1",    /* name of task. */
      10000,       /* Stack size of task */
      NULL,        /* parameter of the task */
      1,           /* priority of the task */
      &task_1,     /* Task handle to keep track of created task */
      0);          /* pin task to core 0 */

  // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      task_2_code, /* Task function. */
      "task_2",    /* name of task. */
      10000,       /* Stack size of task */
      NULL,        /* parameter of the task */
      1,           /* priority of the task */
      &task_2,     /* Task handle to keep track of created task */
      1);          /* pin task to core 1 */
}

// task 1 code
void task_1_code(void *pvParameters)
{

  // debug for core use
  Serial.print("task 1 running on core ");
  Serial.println(xPortGetCoreID());

  while (true)
  {
    Serial.println("hello from task 1");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// task 2 code
void task_2_code(void *pvParameters)
{

  // debug for core use
  Serial.print("task 2 running on core ");
  Serial.println(xPortGetCoreID());

  while (true)
  {
    Serial.println("hello from task 2");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

// loop function
void loop()
{
  // nothing to see in here
}