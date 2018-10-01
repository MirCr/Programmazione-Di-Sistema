# Programmazione-Di-Sistema
Progetto del corso di Programmazione Di Sistema: Condivisione di file tramite rete LAN.

## Comandi Utili
### Importare progetto su cartella nuova (pull)
Usando una Git bash ed entrando nella cartella desiderata, digitare:
```
git init -> istanzio un nuovo Git repo vuoto
git remote add origin https://github.com/MirCr/Programmazione-Di-Sistema -> aggiungo un collegamento remoto al repository del progetto
git pull origin master -> pull del progetto
```

### Aggiornare repository (push)
Usando una Git bash ed entrando nella cartella desiderata, digitare:
```
git add . -> aggiungo tutti gli elementi del repository attuale e li preparo al commit
git commit -m "nuovo commit" -> Faccio il commit dei cambiamenti effettuati e li preparo ad essere spediti verso un repository remoto
git push origin master -> push del progetto verso repository remoto
```

### Creare un nuovo branch e lavorare con esso
```
 (master) è il branch originario.
 git branch "nome_branch" -> crea un nuovo branch chiamato "nome_branch".
 git branch -> visualizza l'elenco dei branch (master branch compreso).
 git checkout "nome_branch" -> switch dal branch corrente al branch "nome_branch".
 git -> pull origin "nome_branch" -> pull del progetto sul branch "nome_branch".
 git -> push origin "nome_branch" -> push del progetto sul branch "nome_branch". 
 ```
