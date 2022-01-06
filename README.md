# ESERCIZIO INCROCIO
### Presentazione
1. Ci sono 4 strade a due corsie e doppio senso di marcia **numerate da 0 a 3** in senso antiorario che confluiscono in un **incrocio**.
2. Le auto girano **sempre** a destra e proseguono.
3. Ogni auto impiega **2 secondi a disimpiegare l'incrocio**
4. Al massimo **2 automobili per volta** possono impiegare l'incrocio

### Regole di precedenza
Per evitare incidenti esiste una **regola di precedenza**:
**l'auto che proviene dalla i-esima strada ha la precedenza sulle strade di indice maggiore**.

### Code FIFO
Ciascuna strada può mantenere più automobili in attesa per volta, affinché le cose siano eque occorre che tali code siano di tipo **FIFO**: la **prima** automobile **che arriva** sarà anche **la prima ad attraversare** all'incrocio.
