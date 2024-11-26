# PIN changer #

Doplnkovy program pre projekt *client_server_hidden* sluzi na zabezpecenie 
zdielaneho tajomstva PIN-kodom na klientskej strane.

2024-26-10, v.1.0.1, Nikita Kuropatkin KEMT FEI TUKE

Program ponuka dve moznosti spravy zdielaneho kluca:

1. **Zabezpecenie cisteho kluca**  
   Program pouzije kluc z premennej `secured_key` v adresari `src/client` 
   a zasifruje ho PIN-kodom.

2. **Zmena PIN-kodu existujuceho kluca**  
   Program zoberie kluc z premennej `secured_key`, pouzije stary PIN na jeho 
   desifrovanie a nasledne ho zasifruje novym PIN-kodom. Pouzivatel musi zadat 
   stary aj novy PIN.

Tento program pomaha klientovi bezpecne pracovat so zdielanym tajomstvom.

## Konfiguracia

Adresar `pin_changer` obsahuje zdrojovy subor `pin_changer.c`.  
Na vyber rezimu nastavte makro `MODE`:

- `SECURE_KEY` pre zabezpecenie cisteho kluca
- `CHANGE_PIN` pre zmenu PIN-kodu existujuceho kluca

Podla zvoleneho rezimu nastavte premennu `new_pin` (a `old_pin` pre `CHANGE_PIN`).
Upraveny subor ulozte, skompilujte (navod nizsie) a spustite. Program vypise 
novu hodnotu `secured_key`, ktoru aktualizujte v subore `src/client/secret.h`.

Ak chcete pouzit inu hodnotu soli pre hashovanie (subor `src/pin.c`), najprv 
ulozte cisty kluc do `src/client/secret.h` a zasifrujte ho pomocou PIN.
Pri zmene soli pred spravnou konfiguraciou zabezpeceneho kluca
API prestane fungovat.

## Pouzitie

Program je kompatibilny s Windows a Linux.

### Windows

- GCC 13.2.0  
- MinGW-64 12.0.0  
- Minimalna verzia CMake: 3.10  

### Linux

- GCC 11.4.0  
- Minimalna verzia CMake: 3.10  

## Kompilacia

Existuju dva sposoby kompilacie: cez CMake alebo Make.

### CMake

1. Otvorte terminal v adresari `build`.  
2. Zadajte `cmake ..`.  
3. Po uspesnom vytvoreni build suborov spustite:  
   - Windows: `cmake --build .`  
   - Linux: `make`  
4. Spustitelny subor `pin_changer.exe` sa ulozi do `Execute`s`.

**Poznamka pre Linux**  
Ak sa zobrazi chyba:  
`No CMAKE_CXX_COMPILER could be found`,  
nainstalujte g++:  
`sudo apt install g++`.

### Make

1. Otvorte terminal v adresari projektu.  
2. Zadajte `make`.  
3. Vytvori sa spustitelny subor `pin_changer.exe`.
