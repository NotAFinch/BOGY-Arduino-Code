# BOGY 2026: Smart Car Projekt (EMCL Heidelberg)

Dieses Repository enthält den Quellcode für das Mikrocontroller-Projekt, das im Rahmen meines **BOGY-Praktikums (Berufsorientierung am Gymnasium)** im Mai 2026 entstanden ist.

## Projektbeschreibung

Ziel des Projekts war die Entwicklung und Programmierung eines autonomen Modellfahrzeugs auf Basis eines **Arduino**. Das Fahrzeug wurde so programmiert, dass es:

1. Per **Infrarot-Fernbedienung** gesteuert werden kann.
2. Hindernisse mittels **Ultraschallsensor** erkennt.
3. Über eine Status-Beleuchtung (RGB-LEDs) Feedback zum aktuellen Systemzustand gibt.

### Technische Highlights & Herausforderungen

* **Modulare Programmierung:** Aufteilung des Codes in Header- und Source-Dateien (`DriverFunctions.cpp`, `smartcarDef.h`) zur besseren Wartbarkeit.
* **Physikalische Sensorik:** Auseinandersetzung mit der Reflexion von Schallwellen und der daraus resultierenden Problematik bei der Kantenerkennung in flachen Winkeln (0°–30°).
* **Signalverarbeitung:** Dekodierung von IR-Hex-Signalen zur präzisen Motorsteuerung.

## Repository-Struktur

* `smartcar.ino`: Hauptprogramm (Main Loop und IR-Logik).
* `DriverFunctions.cpp / .h`: Implementierung der Fahrbefehle und LED-Ansteuerung.
* `smartcarDef.h`: Definition der Pin-Belegungen und Signal-Parameter.
* `Hinweis`: Die Datei Struktur entspricht nicht der im Project. Da dieses Repo für nur für meine Quellen relevant ist.

## Verwendete Technologien

* **Hardware:** Arduino Uno, Motortreiber, Ultraschallsensor, IR-Empfänger.
* **Software:** C++ / Arduino IDE.
* **ExternalLibs** Enthalten die Libarys des Herstellers die ich nur minimal bis gar nicht bearbeitet habe.

## Abhängigkeiten (Libraries)

Um diesen Code zu kompilieren, werden die folgenden Arduino-Bibliotheken benötigt:

* **FastLED** (v3.x) - Zur Ansteuerung der RGB-LEDs.
* **IRremote** (v4.x) - Zur Dekodierung der Infrarot-Signale.

## ⚖️ Lizenz

Dieses Projekt ist unter der **MIT-Lizenz** lizenziert – siehe die [LICENSE](LICENSE) Datei für Details.
