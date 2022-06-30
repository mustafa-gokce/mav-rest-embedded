#include "main.h"

// define tasks
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// task declarations
void task_1_code(void *pvParameters);
void task_2_code(void *pvParameters);

// hotspot configurations
const char *ssid = "re-fly";
const char *passphrase = "rescience2609";
IPAddress address(26, 9, 1, 1);
IPAddress gateway(26, 9, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// create the web server
WebServer server(80);

// server declarations
void server_create_json(char *tag, double value, char *unit);
void server_get();

// JSON data buffer
StaticJsonDocument<250> server_response_json;
char server_response_buffer[250];

// setup function
void setup()
{

  // start serial port
  Serial.begin(9600);

  // create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
      task_1_code, // task function
      "task_1",    // name of task
      10000,       // stack size of task
      NULL,        // parameter of the task
      1,           // priority of the task
      &task_1,     // Task handle to keep track of created task
      0);          // pin task to core 0

  // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      task_2_code, // task function
      "task_2",    // name of task
      10000,       // stack size of task
      NULL,        // parameter of the task
      1,           // priority of the task
      &task_2,     // Task handle to keep track of created task
      1);          // pin task to core 1

  // configure hotspot
  WiFi.softAPConfig(address, gateway, subnet);

  // start hotspot
  WiFi.softAP(ssid, passphrase);

  // declare server routes
  server.on("/get", server_get);

  // start the server
  server.begin();
}

// loop function
void loop()
{
  // handle the client
  server.handleClient();
}

// task 1 code
void task_1_code(void *pvParameters)
{

  // debug for core use
  Serial.print("task 1 running on core ");
  Serial.println(xPortGetCoreID());

  // do below always
  while (true)
  {
    // debug for task is running
    Serial.println("hello from task 1");

    // cool down the task loop
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// task 2 code
void task_2_code(void *pvParameters)
{

  // debug for core use
  Serial.print("task 2 running on core ");
  Serial.println(xPortGetCoreID());

  // do below always
  while (true)
  {
    // debug for task is running
    Serial.println("hello from task 2");

    // cool down the task loop
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void server_get()
{
  server_create_json("random", esp_random(), "int");
  server.send(200, "application/json", server_response_buffer);
}

void server_create_json(char *tag, double value, char *unit)
{
  server_response_json.clear();
  server_response_json["type"] = tag;
  server_response_json["value"] = value;
  server_response_json["unit"] = unit;
  serializeJson(server_response_json, server_response_buffer);
}