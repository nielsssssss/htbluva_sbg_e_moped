# E-Moped

In diesem Repository befindet sich der vollständige Quellcode zum HMI-System eines auf Elektroantrieb umgebauten Mopeds.  

Das Projekt wurde im Rahmen des Informatikunterrichts der **4BHEL** an der **HTBLuVA Salzburg** im Schuljahr **2024/25** realisiert.  

## Projektbeschreibung

Ziel war die Entwicklung eines modernen Human-Machine Interfaces (HMI), das in Echtzeit fahrzeugspezifische Daten wie Geschwindigkeit, Batteriestatus oder Leistungsabgabe anzeigt. Die Architektur basiert auf einem **Raspberry Pi 5**, einem **RP2040 Feather Mikrocontroller** sowie einer zuverlässigen **CAN-Bus-Kommunikation**.

Der Raspberry Pi übernimmt die grafische Visualisierung über ein Webinterface (HTML, CSS, JS), während der RP2040 sämtliche systemkritischen Funktionen wie Poti-Auswertung, Button-Terminal und Datenübertragung steuert.

## Inhalte des Repositories

- Python-Skript zur CAN-Datenverarbeitung
- Webinterface (HTML/CSS/JS) für die Anzeige auf dem Touchdisplay
- Firmware für den RP2040 Feather
- Dokumentation zur Systemarchitektur und Datenfluss

## Team

Dieses Projekt wurde umgesetzt von:  
**Dominik Nachbar**  
**Patrick Unger**  
**Niels Steinböck**
