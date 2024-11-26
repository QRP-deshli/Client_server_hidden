# Client_server_hidden #
Program pre sifrovanu komunikaciu klient-server so steganografickou podporou
(Elligator 2 a PADME)
--------------------------------------------------------------------------------
2024-11-10, v.1.0.5, Nikita Kuropatkin KEMT FEI TUKE

Zakladny ciel programu:
---------------------------
Cielom tohto programu je pomocou zakladnych stavebnych prvkov, ktore ponuka
Monocypher, vytvorit system na sifrovanu komunikaciu medzi dvoma stranami.
Program ma zaistit, aby komunikacia bola tazko analyzovatelna a zaroven skryt
samotnu existenciu komunikacie pomocou eliminacie tzv. "metadat".

Pouzite bloky:
------------------
AEAD (ChaCha20 + Poly): Funkcia, ktora zarucuje sifrovanie dat a ich 
autentifikaciu naraz.
Pouzita implementacia vyuziva dva zakladne bloky:

ChaCha20: Prudova sifra, ktora sa pouziva na sifrovanie sprav medzi klientom 
a serverom.

Poly1305: Funkcia na generovanie jednorazovej hodnoty 
MAC (message authentication code), tzv. "tag", na zaistenie integrity 
posielanych dat.

X25519: Algoritmus na vymenu klucov zalozeny na eliptickej krivke Curve25519. V
tomto programe sa pouziva funkcia crypto_x25519_dirty_small, ktorá ku
vygenerovanému verejnému kľúču pridáva bod krivky s nízkym rádom. To umožňuje
generovať verejné kľúče na celej krivke, nielen v tzv. hlavnej podgrupy krivky.
Táto funkcia je použitá kvôli kompatibilite s Elligatorom.

Blake2b: Hasovacia funkcia zvolena na implementaciu KDF (Key Derivation
Function), teda na odvodenie spolocneho kluca po vymene klucov medzi dvoma
stranami.

Argon2i: Funkcia na hashovanie hesiel. Bezi v konstantnom case, co ju robi 
odolnou voci utokom postrannymi kanalami.
Zaroven je narocna na hardware, co vyrazne spomaluje metodu odhadovania hesla 
utokom hrubou silou.

Elligator 2: Tento blok je pouzity na zabezpecenie steganografie v programe.
Verejny kluc, ktory bol vygenerovany, je pomocou Elligatora namapovany na
skalar. Dovodom je, ze verejny kluc (pri sifrovani na zaklade eliptickej krivky
Curve25519) je bod na krivke, ktory ma tri vlastnosti, ktore mozu byt
analyzovane a mozu prezradit, ze prebieha vymena klucov, co je neziaduce. Dve z
tychto moznosti pre analyzu vieme odstranit pomocou Elligatora 2:

(1) Overenie rovnice krivky: V pripade Curve25519 sa to nevyplati, pretoze
nepracujeme s hodnotou "y".
(2) Overenie, ci ma rovnost x^3 + 486662x^2 + x mod
(255^19) odmocninu: Ak posielame bod na krivke, tato rovnost bude mat odmocninu
so 100% pravdepodobnostou, ale ked posielame skalar (pseudo-nahodny), tato
pravdepodobnost klesne na 50%. Preto chceme bod na krivke namapovat na skalar,
ktory nebude davat odpovedajucemu systemu informacie o tom, ze prebieha vymena
klucov.

PADME: Algoritmus na upravu velkosti sprav, ktory eliminuje tretiu moznost
analyzy, a to analyzu velkosti sprav. Aj ked sme pouzili Elligator 2 na skrytie
verejneho kluca a odstranili dve moznosti analyzy, odpovedajuci system moze
stale analyzovat velkosti sprav. Kluce aj nonce pre rozne sifry maju pevne
stanovene velkosti (napr. kluc pre Curve25519 ma velkost 32 bajtov, nonce pre
ChaCha20 ma velkost 24 bajtov). Hoci tieto informacie same o sebe neodhaluju,
ci dochadza k vymene klucov, mozu byt pre nahodneho odpocuvaca indikatorom, ze
prebieha sifrovana komunikacia. PADME tento problem riesi tym, ze zmeni rozmery
sprav a skryje metadata spojene s komunikaciou.

Algoritmus praci programu:
---------------------------
(0) Server vlastni dlhodobo zdielany kluc (SK), klient ma jeho verziu, 
zabezpecenu pomocou PIN-kodu.

(1)Na zaciatku sa vytvori socket pre komunikaciu vo funkcii main().

(2)Potom sa zavola funkcia key_exc_ell, ktora zabezpeci vymenu klucov a
generovanie rovnakeho "shared secret" na oboch stranach v tychto krokoch:
      
      1)Klient si zvoli svoj sukromny kluc (SK), na zaklade ktoreho sa
      vygeneruje verejny kluc (PK). PK sa nasledne pomocou Elligatora namapuje
      na skalar. Po uprave velkosti spravy sa tento skryty PK posle serveru.

      2)Server dostane "skryty" PK od klienta, upravi jeho velkost a potom
      pomocou Elligatora namapuje skalar na prislusny bod na krivke
      (PK klienta).
      
      3)Server potom pomocou funckie key_hidden() vytvori svoj SK a PK, a 
      Elligator namapuje serverovy PK na
      prislusny skalar. Nasledne sa tento skalar, upraveny pomocou PADME, posle
      klientovi.

      4)Klient vykona rovnake kroky ako server v predchadzajucom bode, aby
      ziskal PK servera.

      *Dalsie dva kroky sa vykonaju vo funkcii kdf():*
      5)Pomocou funkcie x25519 obe strany vygeneruju "shared secret", co je
      surovy zdielany kluc.
  
      6)Pomocou funkcie Blake2b sa vygeneruje sifrovaci kluc pre 
      AEAD(ChaCha20 + Poly1305)
      (pri generovani sa pouziju verejne kluce oboch stran a "shared secret").
      *
      (7) Na klientskej strane si program vyziada PIN, ktory pomocou funkcie 
      Argon2i zahasuje a zoxoruje so zabezpecenym zdielanym klucom. 
      Tym ziska dlhodoby zdielany kluc (SK) v cistej podobe, 
      teda bez zabezpecenia pomocou PIN-kodu.
     
      8)Pomocou Blake2b a zdielaneho SK sa vygeneruje MAC, ktory strany 
      preposlu jedna druhej. Tymto overia, ze druha strana je legitimna 
      (kazda strana porovna MAC druhej strany s tym, co vygenerovala samostatne.
      Ak su rovnake, znamena to, ze druha strana vlastni zdielany SK). 
      Ked strany su legitimne kluc vygenerovani pomocou kdf() sa potom pouzije
      na sifrovanie komunikacie.


(3)Po uspesnej vymene klucov sa zavola funkcia chat, kde bude prebiehat samotna
komunikacia:
      
      1)Na zaciatku obe strany vygeneruju nonce a potom ho vyplnia pomocou
      PAMDE a poslu druhej strane
      
      2)Kazda zo stran nainicializuje dva bloky AEAD: jeden pre autentifikovane
      sifrovanie a druhy pre autentifikovane desifrovanie sprav od druhej strany.

      3)Klient ziska spravu z konzoloveho vstupu, spravu zasifruje a autentifikuje
      pomocou AEAD (ChaCha20 sa pouzije na sifrovanie a Poly1305 na generovanie 
      MAC spravy). Nasledne sifrovanu spravu a MAC posle druhej strane.
      
      4)Server desifruje a autentifikuje prijatu spravu pomocou AEAD,
      vykona rovnake kroky ako klient a posle klientovi zasifrovanu spravu a MAC.
      
      5)Komunikacia sa ukonci, ak niektora zo stran posle stop-slovo
      (predvolena hodnota je "exit") alebo ak niektora zo stran ukonci
      beh programu.


Navod na pouzitie:
-------------------
Program je kompatabilny pre Windows a Linux:

#Windows Verzie nastrojov: 
GCC 13.2.0 
MinGW-64 12.0.0
Minimalna verzia cmake: 3.10

#Linux
GCC 11.4.0
Minimalna verzia cmake: 3.10

####################
 Existuju tri sposoby kompilacie: pomocou CMake, Make alebo prikazu GCC.
 
 #CMAKE# 

      1)V adresare build(tam sa ulozia subory potrebne pre kompilaciu)
       otvorte prikazovy riadok. 

      2)Zadajte prikaz: cmake .. 

      3)Po uspesnom vygenerovani build-suborov pouzite prikaz:
       Windows: "cmake --build ." , Linux: "make" 

      4)Po uspesnom vykonani prikazu sa vytvori adresar "Execute`s",
      kde budu ulozene server.exe a client.exe. 

      5)Najprv spustite server a potom klienta.

 !!!!!Problem Linux!!!!! 
  Ked sa vam vypise chybova hlaska:
 -- The CXX compiler identification is unknown CMake 
  Error at CMakeLists.txt:4
  (project): No CMAKE_CXX_COMPILER could be found. 
  To znamena, ze na vasom systeme chyba g++ kompilator:
  Pre Ubuntu: sudo apt install g++

 #####

 #MAKE# 

      1)V adresare projektu otvorte prikazovy riadok. 

      2)Zadajte prikaz:
      make

      3)Po uspesnom vykonani prikazu sa vytvoria spustitelne subory:
      server.exe 
      client.exe 

      4)Najprv spustite server a potom klienta.

 ################
 Moznosti nastavenia parametrov:
 -------------------------------
 # Cislo portu #
 Pouzivatel moze zmenit predvolene cislo portu (8087) zadanim noveho cisla
 ako druheho argumentu pri spusteni programu:
 ./server.exe 8999
 ./client.exe 8999
 (cislo musi byt rovnake na oboch stranach)
 
 # IP adresa servera #
 Pouzivatel ma tiez moznost zmenit predvolenu adresu servera 
 pre klientsku stranu.
 Predvolena adresa je nastavena ako loopback adresa (127.0.0.1).
 Adresu je mozne zmenit zadanim ako tretieho argumentu pri spusteni client.exe:
 ./server.exe 8999
 ./client.exe 8999 192.134.11.22

 # Chybove kody #
  0 - program bol normalne ukonceny (ziadna chyba sa nevyskytla).  
  1 - chyba: nepodarilo sa spustit WSA.  
  2 - chyba: nepodarilo sa vytvorit socket.  
  3 - chyba: nepodarilo sa spojit socket s IP adresou (binding).  
  4 - chyba: nepodarilo sa spustit funkciu listen pre socket.  
  5 - chyba: nepodarilo sa pripojit klienta ku serveru (server accept).  
  6 - chyba: nepodarilo sa nahrat vstup.  
  7 - chyba: nepodarilo sa prijat data.  
  8 - chyba: nepodarilo sa poslat data.  
  9 - chyba: nepodarilo sa vygenerovat nahodne cisla.  
  10 - chyba: klientovi sa nepodarilo pripojit ku serveru.  
  11 - chyba: nespravne zadane cislo portu.  
  12 - chyba: nespravne zadana ip adresa.  
  13 - chyba: sprava bola modifikovana pocas prenosu.  
  14 - chyba: ina strana nie je legetimnou(nevlastni spolocny zdielany kluc).
  15 - chyba: klient zadal nespravny PIN pre SK.
  16 - chyba: alokovanie pamate pre hashovanie zlyhalo.

 ################
# Zdroje #
https://elligator.org/
https://monocypher.org/