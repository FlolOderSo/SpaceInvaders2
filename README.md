Codezusammenfassung
======================
Der Code ist in drei Dateien unterteilt. Main.cpp ist das Main und übernimmt alles, was mit dem Display und der generellen Grundstruktur zu tun hat. In GameLogic.h ist alles, was mit dem Spiel und der
Logik dahinter zu tun hat. In Motor.h liegt alles, was mit der Motorsteuerung und den Knöpfen zu tun hat.

main
-------
Den größten Teil hier macht das Setup und die Einstellung des Displays. Dieser Teil ist aus dem Internet. Wichtig für unser Projekt ist nur, dass ein E-Pin gesetzt wird und die Größe richtig eingestellt
wird (64×64). Ansonsten funktioniert der Code mit dem Display, das wir verwenden, nicht. In void loop wird dann das Game Frame für Frame ausgeführt.

GameLogic
--------------
Das Wichtigste ist die Struktur GameObjekt. Der Spieler, jeder Gegner und jeder Schuss ist so ein Objekt. Alle Objekte werden in einer Liste gespeichert.
Jedes Objekt kann gezeichnet werden (Funktionalität liegt im Main), kann sich bewegen und seine Hitbox in das Spielfeld eintragen.

Motor
---------------
Hier liegt die Logik, um den Motor anzusteuern und um die Knöpfe abzufragen. Implementiert wird das in der Funktion doStuffSpaceship, welche für die Motorbewegung zuständig ist.

Generelle Funktion
---------------------
Das Spiel wird Frame für Frame berechnet. Vor Spielbeginn wird erst ein Konfig-Modus gestartet, in dem man das Modell des Spaceships hin- und herfahren kann, um es zu zentrieren. Danach läuft jeder Frame gleich ab.
Solange der Spieler noch Leben hat, werden erst alle bestehenden Objekte aktualisiert und bewegt. Danach tragen alle Objekte ihre Hitbox in ein zweidimensionales Array ein.
So wird auf Treffer und Kollisionen überprüft. Danach wird mit einer bestimmten Wahrscheinlichkeit ein neuer Gegner erstellt. Danach wird das Display zurückgesetzt und neu gezeichnet.
Falls sich der Spieler in diesem Frame nicht bewegt hat, gibt es noch einen kurzen Delay. Hat der Spieler sich bewegt, gab es den Delay bereits beim Bewegen.
Hat der Spieler keine Leben mehr, wird für 10 Sekunden ein Deathscreen angezeigt, und das Spiel startet wieder beim Zentrieren des Modells.
