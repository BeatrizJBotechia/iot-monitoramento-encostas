# IoT - Monitoramento de Encostas

## Autores

* Beatriz de Jesus Botechia
* Daniella Bizigatth Simplicio da Silva
* Guilherme Morais Silva1, André Luís de Oliveira

Universidade Presbiteriana Mackenzie
Disciplina: Objetos Inteligentes Conectados
2026

---

## Objetivo

Sistema IoT para monitoramento de encostas utilizando ESP32 e MQTT para transmissão de dados em tempo real.
---

## Tecnologias Utilizadas

* ESP32
* MQTT
* HiveMQ
* Wokwi
* MPU6050
* Arduino IDE
---

## Bibliotecas Utilizadas

* Wire.h
* MPU6050.h
* WiFi.h
* PubSubClient.h
---

## Broker MQTT

* Broker: broker.hivemq.com
* Porta: 1883
* Tópico: encosta/dados
---

## Exemplo de Payload MQTT

```
{
  "umidade": 78,
  "inclinacao": 6,
  "status": "ATENCAO"
}
```
---

## Link da Simulação no Wokwi

https://wokwi.com/projects/457492869991865345
---

## Vídeo-Demonstração

Em produção.
