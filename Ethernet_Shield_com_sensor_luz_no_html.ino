/**
 * Projetos Arduino
 * 
 * @author diego patriota
 * @link https://github.com/diegopatriota
 * 19/05/2016 
 */


//----(Bibliotecas)------------------------------------------------------------------------------
#include <SPI.h>
#include <Ethernet.h>


//----(Declaração de variáveis)------------------------------------------------------------------------------
 
//Definicoes de IP, mascara de rede e gateway
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(169,254,158,247);          //Define o endereco IP
IPAddress gateway(192,168,1,1);      //Define o gateway
IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede
 
//Inicializa o servidor web na porta 80
EthernetServer server(80);

//Pino analógico em que o sensor está conectado.
int sensor = A0;      
 
//variável usada para ler o valor do sensor em tempo real.
int valorSensor = 0; 

int potenciometro = 0;

//----(Setup)-----------------------------------------------------------------------------------------
 
void setup()
{
  //Inicializa a interface de rede
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

//-----(Funções)---(Inicio)---------------------------------------------------------------------------------- 
void loop() {
  
  //Aguarda conexao do browser
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
        
    while (client.connected()) {
      if (client.available()) {
        
        char caracter;
  
        //Lendo o valor do sensor.
        int valorSensor = analogRead(sensor);
//---(Sensores)--(Inicio)-------------------------------------------------------------------------------------




//---(Sensores)--(Fim)-------------------------------------------------------------------------------------------                
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == 'n' && currentLineIsBlank) {
          // send a standard http response header
          
//---(HTML)----(Início)---------------------------------------------------------------------------------------          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 2"); //Recarrega a pagina a cada 2seg
          client.println();
          client.println("<!DOCTYPE HTML>");
//---(JavaScript)---------------------------------------------------------------------------------------          
          client.print("<script language= 'JavaScript'>"); 
          client.print("var auto_refresh = setInterval(function(){submitform(); }, 1000);function submitform(){ document.myForm.submit(); } ");
          client.print("</script>");
//---(HTML)--------------------------------------------------------------------------------------------          
          client.println("<html>");
          //Configura o texto e imprime o titulo no browser
          client.print("<font color=#FF0000><b><u>");
          client.print("Projeto Happy chicken");
          client.print("</u></b></font>");
          client.println("<br />");
          client.println("<br />");
          client.print("Valor do Sensor de luminosidade: ");
          client.println(valorSensor);
          client.print("<form method='POST' name='myForm' id='myForm' action='http://localhost/arduino/mysql.php'>");
          //input sensores
          client.print("<input type='hidden' name='temperatura' value='"+String(valorSensor-200)+"' />");
          client.print("<input type='hidden' name='luminosidade' value='"+String(valorSensor)+"' />");
          //---------------------
          client.print("</form>");
          client.println("<br />");
          client.println("<br />");
          client.println("</b></html>");
          
          break;
          
//---(HTML)--(fim)------------------------------------------------------------------------------------------                   
          
        }
        if (c == 'n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != 'r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    }
}


//-----(Funções)---(Inicio)----------------------------------------------------------------------------------
