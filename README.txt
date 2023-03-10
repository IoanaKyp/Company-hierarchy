TEMA 2 SD - IERARHIE
AUTOR: Chiper Ioana-Cristina
GRUPA: 312 CD

CERINTA 1:
    - Implementare HIRE :  Se creeaza angajatul cu numele dat si se cauta pozitia managerului in 
        ierarhia data. Se cauta alfabetic pozitia pe care ar trebui sa intre angajatul in echipa 
        manager-ului si se insereaza angajatul acolo.
    - Implementare FIRE : Se determina angajatul care va fi concediat cautand numele sau in ierarhie. 
        (Aceasta cautare s-a realizat recursiv). Daca angajatul nu are o echipa in subordine, atunci 
        concedierea inseamna doar eliminarea din echipa in care se afla si eliberarea memoriei alocate 
        angajatului in cauza. Daca acesta are o echipa in subordine, atunci, se elimina mai intai 
        angajatul din echipa managerului sau, apoi, pe rand, se insereaza membrii echipei angajatului 
        in echipa managerului (similar cu hire, dar se actualizeaza doar link-urile), iar in final se 
        elibereaza memoria alocata celui concediat.
    - Implementare PROMOTE : Similar cu FIRE, doar ca angajatul care trebuie mutat nu este sters, ci, 
        dupa golirea echipei din subordine si reinserarea ei in echipa manager-ului, este inserat in 
        echipa manager-ului actualului manager.
    - Implementare MOVE_EMPLOYEE : Acelasi principiu ca la PROMOTE, doar ca angajatul este inserat 
        in echipa manager-ului specificat(primit ca parametru)
    - Implementare MOVE_TEAM : Similar cu MOVE_EMPLOYEE, doar ca echipa angajatului nu mai este inserata 
        in echipa managerului curent al angajatului, ci ramane legata de angajat, care va fi inserat in 
        echipa managerului specificat (primit ca parametru)
    - Implementare FIRE_TEAM : Similar cu MOVE_TEAM, doar ca angajatul din varful echipei nu mai este 
        inserat intr-o alta echipa, ci ierarhia ce incepe cu el este stearsa. Stergerea (care implica 
        si elilberarea memoriei folosite) a fost realizata prin intermediul unei functii aditionale. 
        (Observatie: functia DESTROY_TREE a fost implementata la fel).
CERINTA 2:
    - Implementare GET_EMPLOYEES_BY_MANAGER : Ierarhia este parcursa recursiv, iar nodul curent este 
        adaugat intr-o lista. S-a ales folosirea unei liste sortate, pentru a rezolva conditia afisarii 
        angajatilor in ordine alfabetica, sortarea realizandu-se in momentul inserarii.
    - Implementare GET_EMPLOYEES_BY_LEVEL : Pentru gasirea angajatilor de pe un nivel s-a folosit
        o coada. Fiecare angajat din coada initiala (adica de pe nivelul curent) este scos si se adauga in 
        schimb membrii echipei sale. In acest fel, la final, coada va contine angajatii de pe urmatorul
        nivel.
    - Implementare GET_BEST_MANAGER : Similar cu GET_EMPLOYEES_BY_MANAGER, doar ca un angajat este adaugat 
        in lista doar daca are acelasi numar de oameni in echipa sa ca si ceilalti angajati care se afla deja
        in lista, iar daca are mai multi oameni, atunci lista este golita si angajatul este inserat apoi.
CERINTA 3:
    - Implementare REORGANIZE : Incepand cu radacina, nodul curent trece in subordinea nodului din echipa sa 
        care se afla pe calea ce uneste radacina cu angajatul ce va deveni noul varf al ierarhiei. Pentru a 
        putea incepe efectuarea operatiei incepand cu radacina, functia a fost implpementata recursiv.