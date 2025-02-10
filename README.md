Stacja monitorująca warunki wzrostu roślin

Projekt wykorzystuje ESP32 oraz czujniki BME280 i wilgotności gleby.
Dane z czujników są przesyłane do ESP32, a następnie wysyłane do Node-RED, gdzie są filtrowane, uśredniane i przesyłane do chmury na ThingSpeak.
Projekt został stworzony w Arduino IDE.

Projekt obejmuje:
- ESP32 jako mikrokontroler
- Czujnik BME280 do pomiaru temperatury, wilgotności powietrza oraz ciśnienia
- Czujnik wilgotności gleby
- Node-RED do przetwarzania danych
- ThingSpeak do przechowywania i wizualizacji danych

Schematy i zdjęcia poglądowe znajdują się w katalogu [docs](.docs). Plik [.json.json to zapisany diagram przepływu z NodeRed.

Była to moja pierwsza styczność z IoT
