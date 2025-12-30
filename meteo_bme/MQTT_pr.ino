// Функция отправки данных по протоколу MQTT
void MQTT_send(){
   client.loop();
   uint32_t nows = millis();
  if ((nows - lastMsg > settings.off_time * 60000 && settings.mqtt_en)) {
    if(WiFi.status() == WL_CONNECTED) {
        MQTT_send_data("jsondata", JSON_DATA());
     }
   lastMsg = nows; 
  }
 }
void MQTT_send_data(String topic, String data){
         String root = settings.mqtt_topic;
         String top  = root +"/"+ topic;
         String subscr = root +"/control";
         String input = settings.mqtt_serv;
         int colonIndex = input.indexOf(':');
         String ipAddress;
         String port;
         IPAddress Remote_MQTT_IP;

        if (colonIndex != -1) {
             ipAddress = input.substring(0, colonIndex);
             port = input.substring(colonIndex + 1);
             WiFi.hostByName(ipAddress.c_str(), Remote_MQTT_IP);
          }
      
      client.setServer(Remote_MQTT_IP, port.toInt());

      client.setBufferSize(1024); 
           if(client.connected()){                                                                                    // Вторичная отправка данных при подключенном брокере 
          count_rf = 0;
          send_mqtt(top, data, subscr);
           }else{
              count_rf++;
              if (client.connect(ch_id.c_str(), settings.mqtt_user, settings.mqtt_passw)){                            // Первичная отправка данных, выполняестя попытка подключения к брокеру 
                    send_mqtt(top, data, subscr);          
                }else{
                  if(count_rf > 2){                                                                                    // Если были неудачные попытки подключения, то переподключаем Wi-fi
                     WiFi.disconnect();
                     WiFi.begin(settings.mySSID, settings.myPW);
                     count_rf = 0;
                }
            }
        }
     
}


void send_mqtt(String tops, String data, String subscr) {
    if(!annonce_mqtt_discovery) {
        String device_id = "radar_light_switch_" + ch_id;
        String configuration_url = "http://" + WiFi.localIP().toString();
        String top = String(settings.mqtt_topic) + "/jsondata";
        String control = String(settings.mqtt_topic) + "/control";

        // Создаем конфигурацию устройства
        DynamicJsonDocument deviceDoc(512);
        deviceDoc["ids"][0] = device_id;
        deviceDoc["name"] = "Метеостанция";
        deviceDoc["mdl"] = version_code;
        deviceDoc["sw"] = "1.0.0";
        deviceDoc["mf"] = "CYBEREX TECH";
        deviceDoc["configuration_url"] = configuration_url;

        // Вспомогательная функция для публикации конфигурации
        auto publishConfig = [&](const String& type, const String& entity_id, JsonDocument& doc) {
            doc["device"] = deviceDoc.as<JsonObject>();
            String payload;
            serializeJson(doc, payload);
            client.publish(("homeassistant/" + type + "/" + entity_id + "/config").c_str(), 
                          payload.c_str(), true);
        };

        //  Датчик заряда батареи
        {
            DynamicJsonDocument doc(384);
            doc["device_class"] = "voltage";
            doc["name"] = "Заряд батареи";
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "V";
            doc["suggested_display_precision"] = 2; // Округление до двух знаков после запятой
            doc["value_template"] = "{{ value_json.bat }}";
            doc["unique_id"] = ch_id + "_d_bat";
            publishConfig("sensor", ch_id + "_d_bat", doc);
        }

        //  Датчик температуры
        {
            DynamicJsonDocument doc(384);
            doc["device_class"] = "temperature";
            doc["name"] = "Температура";
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "°C";
            doc["suggested_display_precision"] = 2; // Округление до двух знаков после запятой
            doc["value_template"] = "{{ value_json.temp }}";
            doc["unique_id"] = ch_id + "_d_temp";
            publishConfig("sensor", ch_id + "_d_temp", doc);
        }

        //  Датчик влажности
        {
            DynamicJsonDocument doc(384);
            doc["device_class"] = "humidity";
            doc["name"] = "Влажность";
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "%";
            doc["value_template"] = "{{ value_json.hum }}";
            doc["suggested_display_precision"] = 2; // Округление до двух знаков после запятой
            doc["unique_id"] = ch_id + "_d_hum";
            publishConfig("sensor", ch_id + "_d_hum", doc);
        }

        //  Датчик атмосферного давления
        {
            DynamicJsonDocument doc(512);
            doc["device_class"] = "atmospheric_pressure";
            doc["name"] = "Атмосферное давление";
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "mmHg";
            doc["suggested_display_precision"] = 2; // Округление до двух знаков после запятой
            doc["value_template"] = "{{ value_json.pres }}";
            doc["unique_id"] = ch_id + "_d_pres";
            publishConfig("sensor", ch_id + "_d_pres", doc);
        }
        
        //  Время опроса
        {
            DynamicJsonDocument doc(512);
            doc["device_class"] = "duration";
            doc["name"] = "Переодичность отправки";
            doc["state_topic"] = top;
            doc["unit_of_measurement"] = "min";
            doc["value_template"] = "{{ value_json.sllep_t }}";
            doc["unique_id"] = ch_id + "_d_sllep_t";
            publishConfig("sensor", ch_id + "_d_sllep_t", doc);
        }

        annonce_mqtt_discovery = true;
    }
    
    // Отправляем данные 
    client.publish(tops.c_str(), data.c_str());
    client.subscribe(subscr.c_str());
}

String MQTT_status(){
    return client.connected() ? "подключен" : "отключен";
} 
