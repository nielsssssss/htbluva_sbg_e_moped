import asyncio
import json
import websockets

async def send_data(websocket, path):
    # Startwerte der Variablen
    power_absolute = 2 #-10kW - 20kW
    power_relative = 10 #-50 - 100
    battery_percent = 100 #0 - 100
    gear = 1 # N=0 D=1 P=2 R=3
    powermode = 1 # eco=0 comfort=1 sport=2 insane=3
    battery_temp = 56 # 0-100
    motor_temp = 87 # 0-120
    speed = 4664 #0-120km/h

    try:
        while websocket.open:  # Nur senden, wenn die Verbindung offen ist
            motor_temp=motor_temp-1
            # Ausgabe in der Python-Konsole
            print(f"Sending Data...")
            # Daten als JSON senden
            json_data = json.dumps({
                "value1": power_absolute,
                "value2": power_relative,
                "value3": battery_percent,
                "value4": gear,
                "value5": powermode,
                "value6": battery_temp,
                "value7": motor_temp,
                "value8": speed
            })
            await websocket.send(json_data)
            
            # Warte 500ms
            await asyncio.sleep(0.5)
    except websockets.ConnectionClosedOK:
        print("Verbindung wurde korrekt geschlossen.")
    except websockets.ConnectionClosedError as e:
        print(f"Verbindungsfehler: {e}")

start_server = websockets.serve(send_data, "localhost", 6789)

print("WebSocket Server läuft auf ws://localhost:6789")
asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
