# os2017<br/>
<br/>
Kurze Anleitung zur Erstellung von Make und Doc<br/>
<br/>
1) Make:<br/>
Vorlage von fgrep genommen<br/>
Beim Punkt "variables" die Compiler Informationen eingetragen, die definiert waren<br/>
Beim Punkt "targets" myfind eingesetzt wo mygrep verwendet wurde<br/>
Alle Relationen zu Headerfiles gelöscht<br/>
<br/>
Ausführung von Make im Directory des Source-Codes und des Makefiles mittels: make myfind<br/>
<br/>
2) Doc<br/>
Vorlage von fgrep genommen<br/>
Im File doxygen.dcf alle fgreps mit myfind ausgetauscht.<br/>
<br/>
Ausführung folgenden Befehals im Directory des Source-Codes, des Makefiles und des doxygen.dcf: make doc
