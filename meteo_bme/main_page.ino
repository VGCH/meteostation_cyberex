void handleRoot() {
   if (captivePortal()) {  
    return;
  }
  //String header;
  if (!validateToken()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  String html = "<html lang=\"ru\"><head><meta charset=\"UTF-8\"><title>Управление метеостанцией</title>";
  html += "<link href=\"style.css\" rel=\"stylesheet\" type=\"text/css\" />";
  html += "</head><body>";
  html += divcode;
  html += "<h2>Управление метеостанцией</h2>";
  html += "<div class =\"frame\">";
  html += "<text>Время работы устройства:</text> <div class =\"live\" id=\"timew\">00:00:00:00</div>";
  html += "<text>Режим конфигурации завершится через:</text> <div class =\"livePM\" id=\"time_off\">00:00:00:00</div>";
  html += "<text>Статус MQTT </text> <div class =\"live\" id=\"MQTT\">"+MQTT_status()+"</div>";
  html += "<text>Температура, °C: </text>  <div class =\"live\" id=\"temp\" >0.00</div>";
  html += "<text>Влажность воздуха (Rh), %: </text>  <div class =\"live\" id=\"hum\" >0.00</div>";
  html += "<text>Атмосферное давление, mmHg: </text>  <div class =\"live\" id=\"pres\" >0.00</div>";
  html += "<text>Таймер сна, мин: </text>\
  <div class =\"live\">\
  <input name=\"flevel\" id=\"flying\" type=\"range\"\
  min=\"1\" max=\"60\" value=\""+String(settings.off_time)+"\"\
  step=\"1\">\
  </div>";
  html += "<div class =\"live\"><div id=\"response\">"+String(settings.off_time)+"</div></div><br>";
  html += "<text>Напряжение батареи, В: </text>  <div class =\"live\" id=\"bat\" >0.00</div>";
  html += "<br></div></body>";
  html += "<center><br><a href=\"/?page=wlan_config\">Wi-Fi конфигурация</a><br>";
  html += "<a href=\"/?page=send_config\">Настройка передачи данных</a><br>";
  html += "<a href=\"/?page=changelp\">Изменение пароля устройства</a><br>";
  html += "<a href=\"/?page=update_fw\">Обновление прошивки</a><br>";
  html += "<a href=\"#\" onclick=\"rebootdev()\">Перезагрузить устройство</a><br>";
  html += "<a href=\"/login?DISCONNECT=YES\">Выход</a></center>";
  html += "<footer>© <b>CYBEREX TECH</b>, 2025. Версия микро ПО <b>"+version_code+"</b>.</footer>";
  html += "<script src=\"script.js?script=reb_d\"></script>"; 
  html += "</html>";
  server.send(200, "text/html", html);
}
