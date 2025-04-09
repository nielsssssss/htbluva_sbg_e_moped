const connectWebSocket = () => {
    const ws = new WebSocket("ws://localhost:6789");

    ws.onopen = () => {
        console.log("WebSocket verbunden!");
    };

    ws.onmessage = (event) => {
        try {
            const data = JSON.parse(event.data); // JSON-Daten korrekt parsen

            // Jede Variable individuell zuweisen
            document.getElementById("power").textContent = data.value1+'kW'; //power_absolute
            //document.getElementById("batterie").textContent = data.value2; //power_relative
            document.getElementById("batterie").textContent = data.value3+'%'; //battery_percent
            document.getElementById("batter_temp").textContent = data.value6+'°C'; //battery_temp
            document.getElementById("moto_temp").textContent = data.value7+'°C'; //motor_temp
            document.getElementById("speed").textContent = data.value8; //speed

            // Drivemode
            let drivemode;
            let color;

            if (data.value5 === 0) {
                drivemode = "Eco";
                color = "#008000" //green
            } else if (data.value5 === 1) {
                drivemode = "Comfort";
                color = "#ADD8E6" //light_blue
            } else if (data.value5 === 2) {
                drivemode = "Sport";
                color = "#FF0000" //red
            } else if (data.value5 === 3) {
                drivemode = "Insane";
                color = "#FFA500" //orange
            } else {
                drivemode = "Error"; // Falls ein anderer Wert vorliegt
                color = "#FFFFFF" //white
            }
            
            document.getElementById("drivemode").textContent = drivemode;

            let element = document.getElementById('g-r');
            element.style.setProperty('--color', `${color}`);

            element = document.getElementById('g-l');
            element.style.setProperty('--color', `${color}`);

            element = document.getElementById('g-t');
            element.style.setProperty('--color', `${color}`);

            element = document.getElementById('g-b');
            element.style.setProperty('--color', `${color}`);

            element = document.getElementById('drivemode');
            element.style.setProperty('--color', `${color}`);





            //Needle
            let angle = parseFloat(data.value8) * 0.625;
            element = document.getElementById('nadel');
            element.style.setProperty('--needle-angle', `${angle}%`);




            //Gang
            if (data.value4 === 0) {
                element = document.getElementById('N');
                element.style.setProperty('--color', `#FFFFFF`);
                element = document.getElementById('D');
                element.style.setProperty('--color', `#808080`);
                element = document.getElementById('P');
                element.style.setProperty('--color', `#808080`);
                element = document.getElementById('R');
                element.style.setProperty('--color', `#808080`);
            }
            else if (data.value4 === 1) {
                element = document.getElementById('N');
                element.style.setProperty('--color', `#808080`);
                element = document.getElementById('D');
                element.style.setProperty('--color', `#FFFFFF`);
                element = document.getElementById('P');
                element.style.setProperty('--color', `#808080`);
                element = document.getElementById('R');
                element.style.setProperty('--color', `#808080`);
            }
            else if (data.value4 === 2) {
                element = document.getElementById('N');
                element.style.setProperty('--color', `#808080`);
                element = document.getElementById('D');
                element.style.setProperty('--color', `#808080`);
                element = document.getElementById('P');
                element.style.setProperty('--color', `#FFFFFF`);
                element = document.getElementById('R');
                element.style.setProperty('--color', `#808080`);
            }
            else if (data.value4 === 3) {
                element = document.getElementById('N');
                element.style.setProperty('--color', `#808080`);
                element = document.getElementById('D');
                element.style.setProperty('--color', `#808080`);
                element = document.getElementById('P');
                element.style.setProperty('--color', `#808080`);
                element = document.getElementById('R');
                element.style.setProperty('--color', `#FFFFFF`);
            }

            //Powerbar
            if (data.value1 > 0) {
                let pow = parseFloat(data.value1) * 15;
                element = document.getElementById('power_mov_pos');
                element.style.setProperty('--power', `${pow}px`);
                element = document.getElementById('power_mov_neg');
                element.style.setProperty('--power', `0px`);
            }
            else{
                let pow = parseFloat(data.value1) * -20;
                element = document.getElementById('power_mov_neg');
                element.style.setProperty('--power', `${pow}px`);
                element = document.getElementById('power_mov_pos');
                element.style.setProperty('--power', `0px`);
            }


            //Batterybar
            let charge = parseFloat(data.value3) * 4;
            element = document.getElementById('bat_mov');
            element.style.setProperty('--charge', `${charge}px`);


            //Cat
            if (data.value8 > 80) {
                element = document.getElementById('cat');
                element.style.setProperty('--see', `visible`);
            }
            else{
                element = document.getElementById('cat');
                element.style.setProperty('--see', `hidden`);
            }
            


            
        } catch (error) {
            console.error("Fehler beim Parsen der Nachricht:", error);
        }
    };

    ws.onclose = () => {
        console.log("WebSocket geschlossen, versuche erneut...");
        setTimeout(connectWebSocket, 1000); // Automatisch neu verbinden
    };
};

window.onload = connectWebSocket;
