# PIN changer #
Doplnkovy program pre zakladny projekt client_server_hidden pre zabzpecenie 
zdielaneho tajomvstva pomocou PIN-kodu na klientskej strane
--------------------------------------------------------------------------------
2024-26-10, v.1.0.1, Nikita Kuropatkin KEMT FEI TUKE

Program ponuka pouzivatelovi dve moznosti pre zabezpecenie dlhodobeho zdielaneho
kluca pre klientsku stranu:

Zabezpecenie "cisteho" kluca, v tomto rezime pin_checker zoberie kluc z 
premennej secured_key z adresaru src/client a ten zabezpeci pomocou PIN-kodu.

Zmena PIN-kodu pre existujuci kluc, v tomto rezime pin_checker zoberie kluc z 
premennej secured_key z adresaru src/client a ten zabezpeci pomocou noveho 
PIN-kodu, pritom uzivatel musi uviest aj stary PIN ku klucu.
This program helps client to secure key by PIN

Konfiguracia:
Adresar "pin changer" obsahuje zdrojovy subor "pin_changer.c", kde musite 
nastavit makro "MODE" na hodnotu "CHANGE_PIN", zeby zmenit PIN k pred tym
zabezpecenomu klucu, pritom musite nastavit premenne "new_pin" a "old_pin" na 
hodnoty vaseho noveho PIN-kodu a stareho PIN-kodu. Vysledny zdrojovy subor
ulozte, skompilujte(sposoby budu popisane dalej) a spuste, za behu program 
posle na konzolovy vystup premennu "secured_key", ktoru musite aktualizovat v 
src/client/secret.h. Pri opatovnej kompilacii projekt client_server_hidden 
klient bude pouzivat aktualizovanu verziu zabezpeceneho kluca,
teda zabezpecebu novym PINom.
Pri nastaveny hodnoty makra "MODE" na hodnotu "SECURE_KEY" pouzite rezim 
zabezpecenia "cisteho" kluca, musite nastavit premennu secured_key v subore
src/client/secret.h na hodnotu cisteho zdielaneho kluca(ten, ktory pouziva 
server). Taktiez musite nastavit globalnu premennu "new_pin" na hodnotu PINu, 
ktorym chcete zabezpecit kluc.
Subor src/pin.c obsahuje sol pre hashovanie pomocou Argon2i, hodnotu soli
mozete zmenit, ale pritom po tom musite ulolit cisty klluc do 
src/client/secret.h, a ten zabezpecit pomocou PINu, ked pouzite "MODE = 
CHANGE_PIN" Client_Server API nebude fungovat!!!