# TP_Kenny_Mantou_Reseaux
TP Bus et réseaux 1 à 5 

## TP1 - I2C
### 2.1. Capteur BMP280
Mise en œuvre du BMP280

Le BMP280 est un capteur de pression et température développé par Bosch.
1. Les adresses I2C possibles pour ce composant :
   <img width="811" height="314" alt="image" src="https://github.com/user-attachments/assets/332f19c4-f6b0-4601-9cfc-a145bb23c96c" />
2. Le registre 0xD0 "id" permet d'identifier ce composant.
3. Le registre 0xF4 permet de placer le composant en mode normal
4. Les registres 0xA1 à 0x88 contiennent les données concernant l'étalonnage du composant
5. Les registres de 0xFC à 0xFA contiennent les données concernant la température
6. Les registres de 0xF9 à 0xF7 contiennent les données concernant la pression

7. 
<img width="813" height="469" alt="image" src="https://github.com/user-attachments/assets/d5ae9617-da18-42e6-a53d-90cf70c435d7" />

### 2.2. Setup du STM32


### 2.3. Communication I²C

HAL_I2C_Master_Transmit permet d'écire sur le bus, alors que HAL_I2C_Master_Receive permet de lire le bus. Ces 2 fonctions gère le bit R/W, mais il faut quand même lui laisser la place dans l'adresse I²C.
L'adresse I²C est officiellement sur 7 bits. L'API HAL du STM32 demande des adresses I²C sur 8bits, le LSB étant réservé au bit de R/W. Il faudra donc penser à décaler les adresses d'1 bit sur la gauche et laisser le LSB à 0.

#### Communication avec le BMP280
Identification du BMP280 : 
<img width="169" height="43" alt="image" src="https://github.com/user-attachments/assets/64c3c8f6-5393-41d4-abe5-bd4a83d9d405" />

Récupération de l'étalonnage, de la température et de la pression : 


Calcul des températures et des pression compensées


## 3. TP2 - Interfaçage STM32 - Raspberry
### 3.1. Mise en route du Raspberry PI Zéro
Nous avons téléchargé Raspberry Pi OS (32-bit) Lite, configuré le réseau du routeur utilisé en TP et utilisé ssh pour se connecter à notre raspberry.

Le Raspberry a obtenu l'adresse IP en envoyant une requête au serveur (ici la box wi-fi). 




## 4. TP3 - Interface REST
### 4.1. Installation du serveur Python

<img width="397" height="90" alt="image" src="https://github.com/user-attachments/assets/5eec60d8-0266-4ce8-a905-c8f865b43507" />

<img width="458" height="161" alt="image" src="https://github.com/user-attachments/assets/69189998-269f-495f-85af-cae3160a6e55" />

### 4.2. - Première page REST
Le décorateur @app.route sert à associer une route à une fonction Python.
Le fragment est un paramètre de route qui indique que l'URL doit contenir une valeur à cet endroit,que cette valeur est convertie en int et qu'elle sera transmise en argument à welcome.

Quelques tests : 
<img width="475" height="41" alt="image" src="https://github.com/user-attachments/assets/86498f6c-38ee-4154-b846-c7acc9f3400f" /> 

<img width="550" height="175" alt="image" src="https://github.com/user-attachments/assets/c76cd4d3-0d95-4180-8848-8705f3232ced" />

### 4.3. - Nouvelles méthodes HTTP
Méthode POST

<img width="399" height="110" alt="image" src="https://github.com/user-attachments/assets/1ffb1396-f0d6-4113-af22-570ae01c3c1c" />

API CRUD 

<img width="401" height="108" alt="image" src="https://github.com/user-attachments/assets/6374638d-539e-46f5-9603-de055d9902cd" />

<img width="398" height="56" alt="image" src="https://github.com/user-attachments/assets/9a2f4ff5-2399-4262-8528-499430d3f4ba" />

<img width="404" height="78" alt="image" src="https://github.com/user-attachments/assets/f83f0b49-5088-4dae-8d40-9a1717a76d31" />

<img width="393" height="87" alt="image" src="https://github.com/user-attachments/assets/429dc9c6-c550-4439-a9c3-48ca4c3311e5" />

## 5. TP4 - BUS CAN


![WhatsAppVideo2025-12-10at4 31 55PM-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/6c3632fa-3b27-4963-a5d8-641c9c88ae2e)

# 6. TP5 - Intégration I²C - Serial - REST - CAN

<img width="416" height="273" alt="image" src="https://github.com/user-attachments/assets/4143c191-35af-4358-a85c-b5f1dc14e29f" />





