String millis2time() {
    unsigned long totalSeconds = millis() / 1000;
    
    int days = totalSeconds / 86400;           // 86400 = 24 * 3600
    int hours = (totalSeconds % 86400) / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    
    String timeStr = String(days) + ":" + 
                     twoDigits(hours) + ":" + 
                     twoDigits(minutes) + ":" + 
                     twoDigits(seconds);
    return timeStr;
}

String millis2time_minus() {
    // Преобразование оставшихся миллисекунд в формат д:ч:м:с
    const int totalSeconds = 900 - (millis() / 1000);
    
    // Проверка на отрицательное время
    if (totalSeconds <= 0) {
        return "0:00:00:00";
    }
    
    // Вычисление компонентов времени
    const int days = totalSeconds / 86400;
    const int hours = (totalSeconds % 86400) / 3600;
    const int minutes = (totalSeconds % 3600) / 60;
    const int seconds = totalSeconds % 60;
    
    // Форматирование результата
    String result = String(days) + ":";
    result += twoDigits(hours) + ":";
    result += twoDigits(minutes) + ":";
    result += twoDigits(seconds);
    
    return result;
}

 String twoDigits(int digits){
        if(digits < 10) {
          String i = '0'+String(digits);
          return i;
         }else{
          return String(digits);
         }
      }

void time_work(){
   if (captivePortal()) {  
    return;
  }

  if (validateToken()){
    sensor_get();
    StaticJsonDocument<200> doc;

     doc["time"]      = millis2time();
     doc["temp"]      = String(temp);
     doc["hum"]       = String(hum);
     doc["pres"]      = String(pres * 0.75006);
     doc["time_off"]  = millis2time_minus();
     doc["MQTT"]      = MQTT_status();
     doc["bat"]       = String(ESP.getVcc() * 0.001);

     String outjson;
     serializeJson(doc, outjson);
     server.send(200, "text", outjson);    
  }   
}



String JSON_DATA() {
    sensor_get();
    StaticJsonDocument<256> doc;  
    
     doc["temp"]      = String(temp);
     doc["hum"]       = String(hum);
     doc["pres"]      = String(pres * 0.75006); //  В мм.рт.ст
     doc["bat"]       = String(ESP.getVcc() * 0.001);
     doc["sllep_t"]   = String(settings.off_time);
    
    String outjson;
    serializeJson(doc, outjson);
    return outjson;
}
