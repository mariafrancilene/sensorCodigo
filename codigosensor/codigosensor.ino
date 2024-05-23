#include <WiFi.h>
#include <HTTPClient.h>


// -------- CONFIGURAÇÕES DO WI-FI --------
const char* ssid = "Allan Fenelon"; // NOME QUE APARECE NA REDE WI-FI
const char* password = "gpt809070.p@qwy"; // SENHA DA REDE WI-FI
// -------- CONFIGURAÇÕES DO SERVIDOR --------
const char* serverAddress = "brisasensordetector.000webhostapp.com"; // ENDEREÇO DO SERVIDOR (PODE SER O IP OU A DO USADO ATUALMENTE)
const int serverPort = 80; // PORTA PADRÃO


// -------- VALOR LIMITE DE PARA QUE O SENSOR SEJA CONSIDERADO ATIVADO --------
const int limiteSensor = 3500; // O VALOR DO SENSOR VARIA DE 0 A MAIS DE 3500
 

// -------- FUNÇÃO QUE ENVIA O DADO BOOLEANO (TRUE) AO SERVIDOR PHP --------
void enviarDadosParaServidor(bool minhaVariavelBooleana) {
  // VERIFICAÇÃO SE O ESP ESTÁ CONECTADO AO WI-FI
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    // MONTA O ENDEREÇO DE ONDE ESTÁ O ENDERAÇO DO ARQUIVO .PHP PARA O ENVIO DO DADO VIA MÉTODO POST
    http.begin("http://" + String(serverAddress) + ":" + String(serverPort) + "/brisa/deteccao.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "minhaVariavelBooleana=" + String(minhaVariavelBooleana ? "true" : "false");
    int httpCode = http.POST(postData);
    // AQUI VERIFICA APENAS SE FOI POSSÍVEL MANDAR ALGO AO SERVIDOR
    // -- SE NÃO FOR MANDANDO NADA CAIRÁ NO ELSE, CASO CONTRÁRIO RETORNARÁ O CÓDIGO DO ENVIO
    // -- OS CÓDIGOS DE ENVIO PODEM SER FACILMENTE ENCONTRADOS NO GOOGLE, MAS PARA O ENVIO BEM SUCEDIDO É O CÓDIGO 200
    if (httpCode > 0) {
      Serial.printf("[HTTP] Código de resposta: %d\n", httpCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.printf("[HTTP] Falha na conexão: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}
// -------- FUNÇÃO QUE FAZ A CONECÇÃO DO ESP AO WI-FI --------
void conectarInternet(){
  WiFi.begin(ssid, password);
  // ENQUANTO NÃO CONECTAR AO WI-FI NÃO SAIRÁ DESSA FUNÇÃO
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi!");
}


void setup() {

  Serial.begin(115200); // DEVE SER PADRÃO ESSE VALOR
  
  // CHAMA A FUNÇÃO PARA SE CONECTAR A INTERNET
  conectarInternet();
  
  // Configuração do ADC
  analogReadResolution(12); // Define a resolução do ADC para 12 bits ( ISSO AQUI É PADRÃO / NÃO MEXER)


}

void loop() {
  // Lê o valor analógico no pino do sensor
  int valorAnalogico = analogRead(34); // ONDE ESTÁ O CABO DE SINAL DO SENSOR (CABO PRETO)
  
  // SE O VALOR DO SENSOR ULTRAPASSAR UM LIMITE ESTABELECIDO ENTÃO QUER DIZER QUE ELE FOI ATIVADO E ENVIARÁ DADOS AO SERVIDOR
  // FOI ESTABELECIDO O VALOR LIMITE QUE O SENSOR DEVE TER PARA SER ATIVADO DE 3500
  if (valorAnalogico >= limiteSensor) {
    Serial.print("Valor do sensor: ");
    Serial.println(valorAnalogico);
    Serial.println("Enviando dados para o servidor...");
    enviarDadosParaServidor(true);
  }
  
  delay(100); // Espera 5 segundos antes de verificar novamente
}
