# Barbieland — Algoritmi e Strutture Dati 2023/2024

Primo progetto del corso **Algoritmi e Strutture Dati** — Università di Trento, A.A. 2023/2024.

> ⚠️ Repository **privata**. La condivisione pubblica del codice è espressamente vietata dal regolamento del corso.

---

## Il problema

Dato un grafo pesato non orientato che rappresenta il Mondo di Barbie, alcune città intermedie sono occupate dalla M.A.T.T.E.L. La GraphCruise sceglie autonomamente un percorso di costo minimo da Barbieland (nodo `0`) ad Algoritmia (nodo `C-1`).

Si chiede di trovare:

1. **K** — il massimo intero positivo tale che, aumentando il peso di ogni arco di K, nessun percorso di costo minimo passi per una città occupata.
2. **Percorso minimo** con K = 0 (parte facoltativa).

**Output:**
- `K > 0` se esiste un valore massimo valido
- `-1` se K è illimitato (il percorso minimo evita già sempre le città occupate, indipendentemente da K)
- `-2` se è impossibile trovare un K > 0 (ogni percorso minimo passa sempre per una città occupata)

---

## Approccio

Il cuore della soluzione è una BFS/visita sul grafo che calcola simultaneamente:

- `min_dist[h]` — minima distanza tra B e A passando per **almeno un nodo occupato**, usando esattamente `h` archi
- `min_dist_safe[h]` — minima distanza tra B e A passando **solo per nodi liberi**, usando esattamente `h` archi

Con questi due array si costruisce la funzione `ComparaIntervalli`, che determina il massimo K confrontando i costi dei percorsi "pericolosi" e "sicuri" al variare del numero di archi: aggiungendo K a ogni arco, un percorso con `h` archi aumenta di costo `h * K`, quindi si cerca il massimo K per cui i percorsi sicuri restano sempre più convenienti di quelli pericolosi.

---

## Struttura del codice

```
sol-70-bfs-compare.cpp
│
├── bfs(...)              # Visita del grafo: calcola min_dist, min_dist_safe e il percorso minimo
├── ComparaIntervalli(...) # Confronta i due array e restituisce il massimo K valido
└── main()                # Lettura input, chiamata alle funzioni, scrittura output
```

**Classi e strutture principali:**

`bfs` riceve il grafo, i nodi sorgente/destinazione e il vettore dei nodi occupati (`matel`). Mantiene due array di distanze e un array di predecessori per ricostruire il percorso. Traccia anche se il nodo corrente è raggiungibile attraverso un nodo occupato (`on_matel`), distinguendo così i due tipi di percorso.

`ComparaIntervalli` riceve i vettori compressi `md` (percorsi pericolosi) e `mds` (percorsi sicuri), ognuno come lista di coppie `(numero_archi, costo_minimo)`. Per ogni percorso sicuro con `h_s` archi e costo `d_s`, cerca il massimo K tale che non esista nessun percorso pericoloso con `h_d` archi e costo `d_d` che risulti migliore dopo l'aggiunta di K per arco.

---

## Compilazione ed esecuzione

```bash
# Compilazione
g++ -O2 -o barbie sol-70-bfs-compare.cpp

# Esecuzione con file I/O (modalità EVAL)
g++ -O2 -DEVAL -o barbie sol-70-bfs-compare.cpp
./barbie   # legge da input.txt, scrive su output.txt

# Esecuzione con stdin/stdout
./barbie < input.txt
```

---

## Formato input/output

**Input** (`input.txt`):
```
C S          ← numero città e strade
a b w        ← S righe: arco tra a e b con peso w
...
M            ← numero nodi occupati
o            ← M righe: identificativo nodo occupato
...
```

**Output** (`output.txt`):
```
K            ← valore di K (-1, -2, o intero positivo)
R            ← (facoltativo) numero di nodi nel percorso minimo con K=0
n1 n2 ... nR ← (facoltativo) nodi del percorso, in ordine
```

---

## Esempi

| Caso | K output | Note |
|---|---|---|
| Percorso minimo unico, nessun nodo occupato sul cammino | `-1` | K illimitato |
| Tutti i percorsi minimi passano per nodi occupati | `-2` | K impossibile |
| Percorso sicuro di costo 10, pericoloso di costo 35 (4 archi ciascuno) | `24` | K = (35-10)/1 - 1 = 24 |
| Caso generale con percorsi di lunghezze diverse | valore calcolato | vedere testo |

---

## Assunzioni sul grafo

- `2 ≤ C ≤ 1.000`, `0 ≤ S ≤ 10.000`
- Pesi degli archi: `1 ≤ w ≤ 100.000`
- Grafo indiretto, connesso, senza self-loop né archi doppi
- Barbieland (`0`) e Algoritmia (`C-1`) non sono mai occupate
- Limite: 1 secondo, 16 MB di memoria

---

## Punteggio ottenuto

| Casi | Descrizione |
|---|---|
| 6/20 | Tutti gli archi con lo stesso peso |
| 4/20 | Nessuna o tutte le città occupate |
| 4/20 | Percorsi da B ad A tutti disgiunti |
| 6/20 | Casi generali senza limitazioni |

Punteggio massimo: 100 punti (5 per caso completo, 3 per solo parte obbligatoria). Sufficienza: 30 punti.
