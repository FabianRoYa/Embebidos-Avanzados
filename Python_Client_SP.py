#Cliente MQTT para publicar el estado de los topicos de los estados
#Tambien se suscribe al topico de los botones.

import paho.mqtt.client as mqtt

# Define variables para el estado de los semáforos (True para verde, False para rojo)
S1_state = False
S2_state = False
S3_state = False
S4_state = False

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    # Subscribing to topics for sensors and semaphores
    #Sensores/Botones
    client.subscribe("esp32/B1")
    client.subscribe("esp32/B2")
    client.subscribe("esp32/B3")
    client.subscribe("esp32/B4")
    #Semaforos
    #client.subscribe("esp32/S1")
    #client.subscribe("esp32/S2")
    #client.subscribe("esp32/S3")
    #client.subscribe("esp32/S4")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    global S1_state
    global S2_state
    global S3_state
    global S4_state

#Logica para los botones 
    if msg.topic == "esp32/B1" and msg.payload == "1":
        print("Sensor 1 activado")
        client.publish("esp32/S1", str(int(S1_state)))

    elif msg.topic == "esp32/B2" and msg.payload == "1":
        print("Sensor 2 activado")
        client.publish("esp32/S2", str(int(S2_state)))

    elif msg.topic == "esp32/B3" and msg.payload == "1":
        print("Sensor 3 activado")
        client.publish("esp32/S3", str(int(S3_state)))

    elif msg.topic == "esp32/B4" and msg.payload == "1":
        print("Sensor 4 activado")
        client.publish("esp32/S4", str(int(S4_state)))

#Logica para el semaforo
    elif msg.topic == "esp32/S1":
        semaphore1_state = bool(int(msg.payload))
        print("Estado de semáforo 1:", semaphore1_state)

    elif msg.topic == "esp32/S2":
        semaphore2_state = bool(int(msg.payload))
        print("Estado de semáforo 2:", semaphore2_state)

    elif msg.topic == "esp32/S3":
        semaphore3_state = bool(int(msg.payload))
        print("Estado de semáforo 3:", semaphore3_state)

    elif msg.topic == "esp32/S4":
        semaphore4_state = bool(int(msg.payload))
        print("Estado de semáforo 4:", semaphore4_state)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.14", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
