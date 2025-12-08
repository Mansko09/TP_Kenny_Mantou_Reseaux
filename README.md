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

### 3.2. Port Série



## 4. TP3 - Interface REST
### 4.1. Installation du serveur Python


<img width="397" height="90" alt="image" src="https://github.com/user-attachments/assets/5eec60d8-0266-4ce8-a905-c8f865b43507" />

<img width="458" height="161" alt="image" src="https://github.com/user-attachments/assets/69189998-269f-495f-85af-cae3160a6e55" />


