#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// 配置区域↓↓↓↓↓↓ 根据实际情况修改以下参数 ↓↓↓↓↓↓
const char* ssid = "Ruler";       // WiFi名称
const char* password = "04qza2b1902";   // WiFi密码
const char* mqtt_server = "47.122.144.38";  // MQTT Broker地址
const int mqtt_port = 1883;                // MQTT端口（阿里云通常为1883）
const char* mqtt_user = "esp32";          // MQTT认证用户名
const char* mqtt_password = "12345678";        // MQTT认证密码
// 配置区域↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // 使用唯一客户端ID连接
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // 模拟传感器数据（实际使用时替换为真实传感器读数）
  float temperature = 25.5; 
  
  // 创建JSON文档
  DynamicJsonDocument doc(256);
  doc["temperature"] = temperature;
  
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  // 发布到MQTT主题
  client.publish("esp32/normalization/sensor1", jsonBuffer);
  
  Serial.print("Published: ");
  Serial.println(jsonBuffer);
  
  delay(5000); // 10秒发送间隔
}