from flask import Flask, render_template

import time

import paho.mqtt.client as mqtt #import the client

broker_address="broker.mqttdashboard.com" 
client = mqtt.Client() #creating a new instance
client.connect(broker_address) #connect function to conncet to broker

def on_message(client, userdata, message):
    print("message received " ,int(message.payload.decode("utf-8")))
    reading = int(message.payload.decode("utf-8"))
    return reading

client.on_message=on_message 

client.loop_start() #start the loop
print("Gas sensor value :")
client.subscribe("Gas_sensor_data")
time.sleep(4) # wait
client.loop_stop() #stop the loop

client.loop_start() #start the loop
print("temp sensor value")
client.subscribe("temp_sensor_data")
time.sleep(4) # wait
client.loop_stop() #stop the loop

client.loop_start() #start the loop
print("hum sensor value")
client.subscribe("hum_sensor_data")
time.sleep(4) # wait
client.loop_stop() #stop the loop

client.loop_start() #start the loop
print("flame sensor value")
client.subscribe("flame_sesnor_data")
time.sleep(4) # wait
client.loop_stop() #stop the loop



app = Flask(__name__)

app.route('/on_T')
def on_T():
    client.publish("LED_project", "T")
    return render_template('on_T.html')
    
app.route('/on_S')
def on_S():
    client.publish("LED_project", "S")
    return render_template('on_S.html')

app.route('/on_L')
def on_L():
    client.publish("LED_project", "L")
    return render_template('on_L.html')

app.route('/on_W')
def on_W():
    client.publish("LED_project", "W")
    return render_template('on_W.html')

app.route('/Gas_stove')
def Gas():
    client.publish("LED_project", "G")
    return render_template('Gas.html')
    
app.route('/off')
def off():
    return render_template('off.html')

