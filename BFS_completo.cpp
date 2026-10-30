// =====================================================================================
//  BFS COMPLETO - CS2023 Algoritmos y Estructuras de Datos - Semana 12
//  (PDF "Breadth-First Search" + PDF "Implementación de BFS" + representación de grafos)
//
//  COMO USARLO (el main lee primero una palabra que indica qué ejecutar):
//     DEMO  -> ejecuta las demostraciones de TODAS las funciones del pseudocódigo
//     C     -> resuelve el problema C (Equipos Rivales)         leyendo de la entrada
//     D     -> resuelve el problema D (Robot que Gira)
//     E     -> resuelve el problema E (Rompiendo paredes)
//     F     -> resuelve el problema F (Luces Apagadas)
//     G     -> resuelve el problema G (Rompecabezas Deslizante)
//  Para enviar un problema al juez, en main deja solo la línea de ese problema, por ejemplo:
//     int main() { cin.tie(0)->sync_with_stdio(false); resolver_equipos_rivales(); }
//
//  INDICE: pseudocódigo de las diapositivas  ->  función de este archivo
//  -------------------------------------------------------------------------------------
//   Algoritmo 1  BFS(G,s)                          bfs_basico
//   Algoritmo 3  marcar al SACAR (con error)       bfs_marcando_al_sacar_con_error
//   Algoritmo 6  la cola es un arreglo             bfs_cola_arreglo
//   Algoritmo 8  Camino(t)                         reconstruir_camino
//   Algoritmo 10 Contar-Caminos(G,s)               contar_caminos_minimos
//   Algoritmo 12 BFS-Grilla                        bfs_grilla (+ reconstruir_camino_grilla,
//                                                  contar_caminos_grilla)
//   Algoritmo 15 Vecinos-Jarras + BFS de estados   vecinos_jarras, bfs_jarras
//   Escapar del fuego (dos BFS)                    bfs_fuego, escapar_del_fuego
//   Algoritmo 17 BFS-01 (cola doble)               bfs_01 (+ bfs_01_grilla)
//   Algoritmo 9 (sem. 11) Transpuesto              calcular_transpuesto
//   PDF de teoría: varias fuentes                  bfs_varias_fuentes
//                  componentes conexas             etiquetar_componentes_conexas
//                  ¿es bipartito?                  es_bipartito
//                  avanzar capa por capa           bfs_por_capas
//                  distancias HACIA un vértice     distancias_hacia_vertice
//                  escalera de palabras            escalera_de_palabras
//
//  Convenciones: vértices numerados de 1 a n; INF representa "no descubierto" (la "∞"
//  del pseudocódigo); padre = -1 representa "nulo". Compilar con: g++ -std=c++17
// =====================================================================================

#include <bits/stdc++.h>
using namespace std;

// ------------------------------- TIPOS Y CONSTANTES ----------------------------------
using Grafo = vector<vector<long long>>;                       // lista de adyacencia
using GrafoPonderado = vector<vector<pair<long long, long long>>>;  // adj[u] = (v, peso)
using Casilla = pair<long long, long long>;                    // (fila, columna)

const long long INF = (long long)1e18;      // "infinito" = no descubierto
const long long MODULO = 1000000007LL;      // 10^9 + 7
const long long DF[4] = {-1, 1, 0, 0};      // desplazamientos de fila:    arriba, abajo, izq., der.
const long long DC[4] = {0, 0, -1, 1};      // desplazamientos de columna: arriba, abajo, izq., der.

// =====================================================================================
//  SECCIÓN 1: UTILIDADES (construir grafos, grillas e imprimir resultados)
// =====================================================================================

// Crea un grafo con n vértices numerados 1..n (la posición 0 no se usa).
Grafo crear_grafo(long long n) {
    return Grafo(n + 1);
}

// Agrega la arista u-v. En un grafo NO dirigido cada arista va en las dos listas.
void agregar_arista(Grafo &lista_adyacencia, long long u, long long v, bool dirigido) {
    lista_adyacencia[u].push_back(v);
    if (!dirigido) lista_adyacencia[v].push_back(u);
}

// Lee m aristas "u v" de la entrada y arma el grafo no dirigido.
Grafo leer_grafo_no_dirigido(long long n, long long m) {
    Grafo lista_adyacencia = crear_grafo(n);
    for (long long i = 0; i < m; i++) {
        long long u, v;
        cin >> u >> v;
        agregar_arista(lista_adyacencia, u, v, false);
    }
    return lista_adyacencia;
}

// Lee una grilla de n filas, cada una con una cadena de m caracteres.
vector<string> leer_grilla(long long n) {
    vector<string> grilla(n);
    for (long long i = 0; i < n; i++) cin >> grilla[i];
    return grilla;
}

// Revisa los límites de la grilla. SIEMPRE se llama antes de mirar g[f][c].
bool dentro_de_grilla(long long f, long long c, long long filas, long long columnas) {
    return f >= 0 && f < filas && c >= 0 && c < columnas;
}

// Una casilla es del borde si está en la primera/última fila o columna.
bool es_borde(long long f, long long c, long long filas, long long columnas) {
    return f == 0 || f == filas - 1 || c == 0 || c == columnas - 1;
}

string texto_distancia(long long d) {
    return d == INF ? "inf" : to_string(d);
}

// Imprime v[desde..fin] mostrando "inf" para los no descubiertos.
void imprimir_vector(const string &titulo, const vector<long long> &v, long long desde) {
    cout << titulo;
    for (long long i = desde; i < (long long)v.size(); i++) cout << " " << texto_distancia(v[i]);
    cout << "\n";
}

void imprimir_camino(const string &titulo, const vector<long long> &camino) {
    cout << titulo;
    if (camino.empty()) cout << " (no hay camino)";
    for (size_t i = 0; i < camino.size(); i++) cout << (i == 0 ? " " : " -> ") << camino[i];
    cout << "\n";
}

void imprimir_camino_grilla(const string &titulo, const vector<Casilla> &camino) {
    cout << titulo;
    if (camino.empty()) cout << " (no hay camino)";
    for (size_t i = 0; i < camino.size(); i++)
        cout << (i == 0 ? " " : " -> ") << "(" << camino[i].first << "," << camino[i].second << ")";
    cout << "\n";
}

// Algoritmo 9 (semana 11): grafo transpuesto. Invierte el sentido de todas las aristas. O(n + m).
Grafo calcular_transpuesto(const Grafo &lista_adyacencia) {
    Grafo transpuesto(lista_adyacencia.size());
    for (long long u = 1; u < (long long)lista_adyacencia.size(); u++)
        for (long long v : lista_adyacencia[u])
            transpuesto[v].push_back(u);  // u -> v en G  se convierte en  v -> u en G^T
    return transpuesto;
}

// =====================================================================================
//  SECCIÓN 2: BFS EN GRAFOS (PDF de implementación)
// =====================================================================================

// ---- Algoritmo 1: BFS(G, s) - la versión normal --------------------------------------
// dist[v] = INF hace de "no descubierto" y cada vértice recibe su dist AL ENTRAR a la cola.
void bfs_basico(const Grafo &lista_adyacencia, long long s, vector<long long> &dist,
                vector<long long> &padre) {
    long long n = lista_adyacencia.size() - 1;
    dist.assign(n + 1, INF);
    padre.assign(n + 1, -1);           // -1 = nulo

    queue<long long> q;
    dist[s] = 0;
    q.push(s);                          // Enqueue(Q, s)

    while (!q.empty()) {
        long long u = q.front();        // u <- Front(Q)
        q.pop();                        // Dequeue(Q)
        for (long long v : lista_adyacencia[u]) {
            if (dist[v] == INF) {       // v todavía no fue descubierto
                dist[v] = dist[u] + 1;
                padre[v] = u;
                q.push(v);              // se marca al METER
            }
        }
    }
}

// ---- Algoritmo 3: "Con un error" - marcar al SACAR en vez de al meter ----------------
// Solo sirve para mostrar el error: un vértice puede entrar varias veces y dist se pisa.
// entradas_a_cola cuenta cuántos elementos entraron a la cola en total.
void bfs_marcando_al_sacar_con_error(const Grafo &lista_adyacencia, long long s,
                                     vector<long long> &dist, long long &entradas_a_cola) {
    long long n = lista_adyacencia.size() - 1;
    dist.assign(n + 1, INF);
    vector<bool> visto(n + 1, false);
    entradas_a_cola = 0;

    queue<long long> q;
    dist[s] = 0;
    q.push(s);
    entradas_a_cola++;

    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        if (visto[u]) continue;         // las copias repetidas se saltan...
        visto[u] = true;                // ...pero se marcó TARDE (al sacar)
        for (long long v : lista_adyacencia[u]) {
            if (!visto[v]) {
                dist[v] = dist[u] + 1;  // puede sobreescribir una distancia ya correcta
                q.push(v);
                entradas_a_cola++;
            }
        }
    }
}

// ---- Algoritmo 6: la cola es un arreglo cola[0..n-1] ---------------------------------
// Cada vértice entra una sola vez, así que n casillas alcanzan y no hace falta cola
// circular: ini y fin solo avanzan. Devuelve cola[0..fin-1] = orden de visita de BFS.
vector<long long> bfs_cola_arreglo(const Grafo &lista_adyacencia, long long s,
                                   vector<long long> &dist, vector<long long> &padre) {
    long long n = lista_adyacencia.size() - 1;
    dist.assign(n + 1, INF);
    padre.assign(n + 1, -1);

    vector<long long> cola(n);          // la cola es un arreglo
    long long ini = 0, fin = 0;

    dist[s] = 0;
    cola[fin++] = s;                    // cola[0] <- s, ini <- 0, fin <- 1

    while (ini < fin) {                 // la cola no está vacía
        long long u = cola[ini++];      // u <- cola[ini], ini <- ini + 1
        for (long long v : lista_adyacencia[u]) {
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                padre[v] = u;
                cola[fin++] = v;        // cola[fin] <- v, fin <- fin + 1
            }
        }
    }
    cola.resize(fin);                   // solo los vértices alcanzados
    return cola;
}

// ---- Algoritmo 8: Camino(t) - después de BFS(s) --------------------------------------
// Sigue padre desde t hasta s y da vuelta la lista. Cuesta O(dist[t]), no O(n).
// Devuelve una lista vacía si no hay camino.
vector<long long> reconstruir_camino(long long t, const vector<long long> &dist,
                                     const vector<long long> &padre) {
    vector<long long> camino;
    if (dist[t] == INF) return camino;  // "no hay camino"
    for (long long v = t; v != -1; v = padre[v])  // padre[s] = nulo detiene el ciclo en s
        camino.push_back(v);
    reverse(camino.begin(), camino.end());        // "devolver c al revés"
    return camino;
}

// ---- Algoritmo 10: Contar-Caminos(G, s) - el mismo BFS, con cnt -----------------------
// cnt[v] = cantidad de caminos mínimos de s a v. Si modulo > 0 se trabaja módulo "modulo".
// OJO: el orden de los dos "si" importa (primero descubrir, luego sumar).
void contar_caminos_minimos(const Grafo &lista_adyacencia, long long s, vector<long long> &dist,
                            vector<long long> &cnt, long long modulo) {
    long long n = lista_adyacencia.size() - 1;
    dist.assign(n + 1, INF);
    cnt.assign(n + 1, 0);

    queue<long long> q;
    dist[s] = 0;
    cnt[s] = 1;
    q.push(s);

    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        for (long long v : lista_adyacencia[u]) {
            if (dist[v] == INF) {           // primer si: descubrir
                dist[v] = dist[u] + 1;
                q.push(v);
            }
            if (dist[v] == dist[u] + 1) {   // segundo si: u está en la capa anterior a v
                cnt[v] += cnt[u];           // cnt[u] ya está completo cuando u sale
                if (modulo > 0) cnt[v] %= modulo;
            }
        }
    }
}

// =====================================================================================
//  SECCIÓN 3: BFS EN GRILLAS (los vecinos se calculan al vuelo)
// =====================================================================================

// ---- Algoritmo 12: BFS-Grilla - paredes '#', se llega con 4 movimientos ---------------
// dist[f][c] = INF si no se llega. padre[f][c] = casilla anterior ({-1,-1} = nulo).
void bfs_grilla(const vector<string> &grilla, Casilla inicio, vector<vector<long long>> &dist,
                vector<vector<Casilla>> &padre) {
    long long filas = grilla.size(), columnas = grilla[0].size();
    dist.assign(filas, vector<long long>(columnas, INF));
    padre.assign(filas, vector<Casilla>(columnas, {-1, -1}));

    queue<Casilla> q;
    dist[inicio.first][inicio.second] = 0;
    q.push(inicio);

    while (!q.empty()) {
        Casilla actual = q.front();
        q.pop();
        long long f = actual.first, c = actual.second;
        for (int k = 0; k < 4; k++) {
            long long nf = f + DF[k], nc = c + DC[k];
            // El orden importa: fuera de la grilla NO se mira g[nf][nc].
            if (dentro_de_grilla(nf, nc, filas, columnas) && grilla[nf][nc] != '#' &&
                dist[nf][nc] == INF) {
                dist[nf][nc] = dist[f][c] + 1;
                padre[nf][nc] = actual;
                q.push({nf, nc});
            }
        }
    }
}

// Igual que reconstruir_camino pero para casillas. Vacío si no se llega al destino.
vector<Casilla> reconstruir_camino_grilla(Casilla destino, const vector<vector<long long>> &dist,
                                          const vector<vector<Casilla>> &padre) {
    vector<Casilla> camino;
    if (dist[destino.first][destino.second] == INF) return camino;
    for (Casilla v = destino; v.first != -1; v = padre[v.first][v.second]) camino.push_back(v);
    reverse(camino.begin(), camino.end());
    return camino;
}

// BFS de grilla que además cuenta los caminos mínimos (mismo cnt del Algoritmo 10).
void contar_caminos_grilla(const vector<string> &grilla, Casilla inicio,
                           vector<vector<long long>> &dist, vector<vector<long long>> &cnt,
                           long long modulo) {
    long long filas = grilla.size(), columnas = grilla[0].size();
    dist.assign(filas, vector<long long>(columnas, INF));
    cnt.assign(filas, vector<long long>(columnas, 0));

    queue<Casilla> q;
    dist[inicio.first][inicio.second] = 0;
    cnt[inicio.first][inicio.second] = 1;
    q.push(inicio);

    while (!q.empty()) {
        Casilla actual = q.front();
        q.pop();
        long long f = actual.first, c = actual.second;
        for (int k = 0; k < 4; k++) {
            long long nf = f + DF[k], nc = c + DC[k];
            if (!dentro_de_grilla(nf, nc, filas, columnas) || grilla[nf][nc] == '#') continue;
            if (dist[nf][nc] == INF) {                  // descubrir
                dist[nf][nc] = dist[f][c] + 1;
                q.push({nf, nc});
            }
            if (dist[nf][nc] == dist[f][c] + 1) {       // sumar caminos de la capa anterior
                cnt[nf][nc] += cnt[f][c];
                if (modulo > 0) cnt[nf][nc] %= modulo;
            }
        }
    }
}

// =====================================================================================
//  SECCIÓN 4: GRAFOS DE ESTADOS (el grafo no se construye) - las jarras
// =====================================================================================

// Numera el estado (a, b) como a * (B + 1) + b, igual que las casillas de una grilla.
long long numerar_estado_jarras(long long a, long long b, long long B) {
    return a * (B + 1) + b;
}

// ---- Algoritmo 15: Vecinos-Jarras(a, b) - capacidades A y B --------------------------
// Devuelve los 6 estados a los que se llega con una jugada.
vector<Casilla> vecinos_jarras(long long a, long long b, long long A, long long B) {
    vector<Casilla> vecinos;
    vecinos.push_back({A, b});   // llenar la 1ra
    vecinos.push_back({a, B});   // llenar la 2da
    vecinos.push_back({0, b});   // vaciar la 1ra
    vecinos.push_back({a, 0});   // vaciar la 2da
    long long t = min(a, B - b);               // pasar de la 1ra a la 2da
    vecinos.push_back({a - t, b + t});
    t = min(b, A - a);                         // pasar de la 2da a la 1ra
    vecinos.push_back({a + t, b - t});
    return vecinos;
}

// BFS desde (0,0) hasta un estado con "objetivo" litros en alguna jarra.
// Devuelve la cantidad mínima de jugadas (o -1) y deja en "camino" los estados recorridos.
long long bfs_jarras(long long A, long long B, long long objetivo, vector<Casilla> &camino) {
    long long total = (A + 1) * (B + 1);
    vector<long long> dist(total, INF), padre(total, -1);

    queue<long long> q;
    long long inicio = numerar_estado_jarras(0, 0, B);
    dist[inicio] = 0;
    q.push(inicio);

    while (!q.empty()) {
        long long id = q.front();
        q.pop();
        long long a = id / (B + 1), b = id % (B + 1);   // se decodifica el estado

        if (a == objetivo || b == objetivo) {           // BFS garantiza que es el mínimo
            camino.clear();
            for (long long x = id; x != -1; x = padre[x])
                camino.push_back({x / (B + 1), x % (B + 1)});
            reverse(camino.begin(), camino.end());
            return dist[id];
        }
        for (Casilla vecino : vecinos_jarras(a, b, A, B)) {
            long long v = numerar_estado_jarras(vecino.first, vecino.second, B);
            if (dist[v] == INF) {
                dist[v] = dist[id] + 1;
                padre[v] = id;
                q.push(v);
            }
        }
    }
    return -1;
}

// =====================================================================================
//  SECCIÓN 5: ESCAPAR DEL FUEGO (dos BFS: el primero con varias fuentes)
//  Grilla: '#' pared, 'F' casilla en llamas, 'P' persona, '.' libre.
// =====================================================================================

// Fase 1: BFS con TODAS las casillas en llamas como fuentes (dist 0).
// fuego[f][c] = minuto en que empieza a arder la casilla (INF si nunca arde).
vector<vector<long long>> bfs_fuego(const vector<string> &grilla, const vector<Casilla> &fuentes) {
    long long filas = grilla.size(), columnas = grilla[0].size();
    vector<vector<long long>> fuego(filas, vector<long long>(columnas, INF));

    queue<Casilla> q;
    for (Casilla fuente : fuentes) {        // todas las fuentes entran al inicio con 0
        fuego[fuente.first][fuente.second] = 0;
        q.push(fuente);
    }
    while (!q.empty()) {
        Casilla actual = q.front();
        q.pop();
        long long f = actual.first, c = actual.second;
        for (int k = 0; k < 4; k++) {
            long long nf = f + DF[k], nc = c + DC[k];
            if (dentro_de_grilla(nf, nc, filas, columnas) && grilla[nf][nc] != '#' &&
                fuego[nf][nc] == INF) {
                fuego[nf][nc] = fuego[f][c] + 1;
                q.push({nf, nc});
            }
        }
    }
    return fuego;
}

// Fase 2: BFS de la persona. Solo entra a una casilla si llega ESTRICTAMENTE antes que el fuego:
//    dist[f][c] + 1 < fuego[f'][c']
// La primera casilla del borde que sale de la cola es la mejor salida; salir del tablero cuesta
// un minuto más, por eso se devuelve dist + 1. Devuelve -1 si es imposible escapar.
long long escapar_del_fuego(const vector<string> &grilla, Casilla persona,
                            const vector<vector<long long>> &fuego) {
    long long filas = grilla.size(), columnas = grilla[0].size();
    if (fuego[persona.first][persona.second] == 0) return -1;  // ya está en llamas

    vector<vector<long long>> dist(filas, vector<long long>(columnas, INF));
    queue<Casilla> q;
    dist[persona.first][persona.second] = 0;
    q.push(persona);

    while (!q.empty()) {
        Casilla actual = q.front();
        q.pop();
        long long f = actual.first, c = actual.second;
        if (es_borde(f, c, filas, columnas)) return dist[f][c] + 1;   // sale del tablero
        for (int k = 0; k < 4; k++) {
            long long nf = f + DF[k], nc = c + DC[k];
            if (dentro_de_grilla(nf, nc, filas, columnas) && grilla[nf][nc] != '#' &&
                dist[nf][nc] == INF && dist[f][c] + 1 < fuego[nf][nc]) {
                dist[nf][nc] = dist[f][c] + 1;
                q.push({nf, nc});
            }
        }
    }
    return -1;
}

// Junta las dos fases: busca las 'F' y la 'P' en la grilla y devuelve los minutos mínimos.
long long resolver_escapar_del_fuego(const vector<string> &grilla) {
    vector<Casilla> fuentes;
    Casilla persona = {-1, -1};
    for (long long f = 0; f < (long long)grilla.size(); f++) {
        for (long long c = 0; c < (long long)grilla[f].size(); c++) {
            if (grilla[f][c] == 'F') fuentes.push_back({f, c});
            if (grilla[f][c] == 'P') persona = {f, c};
        }
    }
    vector<vector<long long>> fuego = bfs_fuego(grilla, fuentes);
    return escapar_del_fuego(grilla, persona, fuego);
}

// =====================================================================================
//  SECCIÓN 6: BFS 0-1 (pesos 0 y 1 con cola doble)
// =====================================================================================

// ---- Algoritmo 17: BFS-01(G, s) -------------------------------------------------------
// Peso 0 -> Push_Front (misma distancia). Peso 1 -> Push_Back (distancia + 1).
// Ya no se pregunta "¿fue descubierto?" sino "¿esta ruta mejora la distancia?".
// Un vértice puede entrar hasta 2 veces, y el costo sigue siendo O(n + m).
void bfs_01(const GrafoPonderado &lista_adyacencia, long long s, vector<long long> &dist) {
    long long n = lista_adyacencia.size() - 1;
    dist.assign(n + 1, INF);

    deque<long long> D;
    dist[s] = 0;
    D.push_back(s);

    while (!D.empty()) {
        long long u = D.front();
        D.pop_front();
        for (pair<long long, long long> arista : lista_adyacencia[u]) {
            long long v = arista.first, w = arista.second;   // arista (u, v, w), w en {0,1}
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) D.push_front(v);
                else        D.push_back(v);
            }
        }
    }
}

// BFS 0-1 sobre una grilla: entrar a una casilla libre cuesta 0 y entrar a una pared '#' cuesta 1.
// dist[f][c] = menor cantidad de paredes que hay que romper para llegar desde "inicio".
void bfs_01_grilla(const vector<string> &grilla, Casilla inicio, vector<vector<long long>> &dist) {
    long long filas = grilla.size(), columnas = grilla[0].size();
    dist.assign(filas, vector<long long>(columnas, INF));

    deque<Casilla> D;
    dist[inicio.first][inicio.second] = 0;
    D.push_back(inicio);

    while (!D.empty()) {
        Casilla actual = D.front();
        D.pop_front();
        long long f = actual.first, c = actual.second;
        for (int k = 0; k < 4; k++) {
            long long nf = f + DF[k], nc = c + DC[k];
            if (!dentro_de_grilla(nf, nc, filas, columnas)) continue;
            long long w = (grilla[nf][nc] == '#') ? 1 : 0;   // el peso depende de la casilla destino
            if (dist[f][c] + w < dist[nf][nc]) {
                dist[nf][nc] = dist[f][c] + w;
                if (w == 0) D.push_front({nf, nc});
                else        D.push_back({nf, nc});
            }
        }
    }
}

// =====================================================================================
//  SECCIÓN 7: USOS DE BFS DEL PDF DE TEORÍA
// =====================================================================================

// Varias fuentes: todas entran a la cola al inicio con dist 0 y se corre UN solo BFS.
// dist[v] = distancia a la fuente más cercana. O(n + m).
void bfs_varias_fuentes(const Grafo &lista_adyacencia, const vector<long long> &fuentes,
                        vector<long long> &dist) {
    long long n = lista_adyacencia.size() - 1;
    dist.assign(n + 1, INF);

    queue<long long> q;
    for (long long fuente : fuentes) {
        dist[fuente] = 0;
        q.push(fuente);
    }
    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        for (long long v : lista_adyacencia[u]) {
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}

// Componentes conexas: un BFS desde cada vértice aún sin etiquetar; todo lo que alcanza recibe
// la misma etiqueta (1, 2, 3, ...). Devuelve la cantidad de componentes. O(n + m) en total.
long long etiquetar_componentes_conexas(const Grafo &lista_adyacencia,
                                        vector<long long> &componente) {
    long long n = lista_adyacencia.size() - 1;
    componente.assign(n + 1, 0);        // 0 = sin etiquetar
    long long cantidad = 0;

    for (long long s = 1; s <= n; s++) {
        if (componente[s] != 0) continue;
        cantidad++;
        queue<long long> q;
        componente[s] = cantidad;
        q.push(s);
        while (!q.empty()) {
            long long u = q.front();
            q.pop();
            for (long long v : lista_adyacencia[u]) {
                if (componente[v] == 0) {
                    componente[v] = cantidad;
                    q.push(v);
                }
            }
        }
    }
    return cantidad;
}

// ¿Es bipartito? BFS coloreando cada vértice con el color contrario al de su padre.
// Si una arista une dos vértices del mismo color, hay un ciclo de largo impar => NO es bipartito.
// (Revisa todas las componentes.) Deja en color[v] el lado (0 o 1) de cada vértice.
bool es_bipartito(const Grafo &lista_adyacencia, vector<long long> &color) {
    long long n = lista_adyacencia.size() - 1;
    color.assign(n + 1, -1);            // -1 = sin color

    for (long long s = 1; s <= n; s++) {
        if (color[s] != -1) continue;
        queue<long long> q;
        color[s] = 0;
        q.push(s);
        while (!q.empty()) {
            long long u = q.front();
            q.pop();
            for (long long v : lista_adyacencia[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false;       // arista dentro de la misma capa/lado
                }
            }
        }
    }
    return true;
}

// Avanzar capa por capa: con la capa k completa se arma la capa k+1 (vecinos aún no descubiertos).
// Devuelve capas[k] = vértices a distancia k de s. Sirve para contar vértices a cada distancia.
vector<vector<long long>> bfs_por_capas(const Grafo &lista_adyacencia, long long s) {
    long long n = lista_adyacencia.size() - 1;
    vector<bool> descubierto(n + 1, false);
    vector<vector<long long>> capas;

    descubierto[s] = true;
    capas.push_back({s});               // capa 0 = {s}
    while (true) {
        vector<long long> siguiente;
        for (long long u : capas.back()) {
            for (long long v : lista_adyacencia[u]) {
                if (!descubierto[v]) {
                    descubierto[v] = true;
                    siguiente.push_back(v);
                }
            }
        }
        if (siguiente.empty()) break;
        capas.push_back(siguiente);
    }
    return capas;
}

// Grafos dirigidos: distancia de TODOS los vértices HACIA t = BFS desde t en el grafo transpuesto.
// (Un camino v -> ... -> t en G es un camino t -> ... -> v en G^T.) Un solo BFS, no uno por vértice.
void distancias_hacia_vertice(const Grafo &lista_adyacencia, long long t, vector<long long> &dist) {
    Grafo transpuesto = calcular_transpuesto(lista_adyacencia);
    vector<long long> padre;
    bfs_basico(transpuesto, t, dist, padre);
}

// Escalera de palabras: el grafo NO se construye; los vecinos de una palabra se generan cambiando
// una letra y consultando el diccionario (tabla hash). Devuelve el camino de palabras (vacío si no hay).
vector<string> escalera_de_palabras(const unordered_set<string> &diccionario, const string &inicio,
                                    const string &fin) {
    unordered_map<string, long long> dist;
    unordered_map<string, string> padre;
    queue<string> q;
    dist[inicio] = 0;
    q.push(inicio);

    while (!q.empty()) {
        string u = q.front();
        q.pop();
        if (u == fin) break;
        for (size_t i = 0; i < u.size(); i++) {         // cada posición...
            for (char letra = 'A'; letra <= 'Z'; letra++) {   // ...con cada otra letra (25 por posición)
                if (letra == u[i]) continue;
                string v = u;
                v[i] = letra;
                if (diccionario.count(v) && !dist.count(v)) {
                    dist[v] = dist[u] + 1;
                    padre[v] = u;
                    q.push(v);
                }
            }
        }
    }

    vector<string> camino;
    if (!dist.count(fin)) return camino;
    for (string x = fin;; x = padre[x]) {
        camino.push_back(x);
        if (x == inicio) break;
    }
    reverse(camino.begin(), camino.end());
    return camino;
}

// =====================================================================================
//  SECCIÓN 8: PROBLEMA C - EQUIPOS RIVALES (componentes + bipartito)
// =====================================================================================

long long potencia_modular(long long base, long long exponente, long long modulo) {
    long long resultado = 1;
    base %= modulo;
    while (exponente > 0) {
        if (exponente & 1) resultado = resultado * base % modulo;
        base = base * base % modulo;
        exponente >>= 1;
    }
    return resultado;
}

// Si el grafo de rivalidades no es bipartito -> "NO". Si lo es, cada componente conexa se puede
// repartir de 2 formas (los aislados también son una componente): "SI 2^componentes mod 1e9+7".
string decidir_equipos_rivales(const Grafo &lista_adyacencia) {
    vector<long long> color;
    if (!es_bipartito(lista_adyacencia, color)) return "NO";
    vector<long long> componente;
    long long cantidad = etiquetar_componentes_conexas(lista_adyacencia, componente);
    return "SI " + to_string(potencia_modular(2, cantidad, MODULO));
}

void resolver_equipos_rivales() {
    long long n, m;
    cin >> n >> m;
    Grafo lista_adyacencia = leer_grafo_no_dirigido(n, m);   // las rivalidades repetidas no molestan
    cout << decidir_equipos_rivales(lista_adyacencia) << "\n";
}

// =====================================================================================
//  SECCIÓN 9: PROBLEMA D - ROBOT QUE GIRA (grafo de estados: fila, columna, dirección)
// =====================================================================================

// Direcciones en sentido horario: 0 arriba, 1 derecha, 2 abajo, 3 izquierda.
// Girar a la derecha = (d + 1) % 4, girar a la izquierda = (d + 3) % 4.
long long codificar_estado_robot(long long f, long long c, long long d, long long columnas) {
    return (f * columnas + c) * 4 + d;
}

// Menor cantidad de acciones (avanzar / girar izquierda / girar derecha) de (0,0) mirando a la
// derecha hasta (n-1, m-1) en cualquier dirección. -1 si es imposible. O(n * m).
long long bfs_robot_que_gira(const vector<string> &grilla) {
    long long filas = grilla.size(), columnas = grilla[0].size();
    const long long df_dir[4] = {-1, 0, 1, 0};
    const long long dc_dir[4] = {0, 1, 0, -1};

    long long total = filas * columnas * 4;
    vector<long long> dist(total, INF);
    vector<long long> cola(total);          // la cola como arreglo (ini / fin)
    long long ini = 0, fin = 0;

    long long inicio = codificar_estado_robot(0, 0, 1, columnas);   // mirando a la derecha
    dist[inicio] = 0;
    cola[fin++] = inicio;

    while (ini < fin) {
        long long u = cola[ini++];
        long long d = u % 4;                // se decodifica el estado
        long long celda = u / 4;
        long long c = celda % columnas, f = celda / columnas;

        // Acción 1: girar a la izquierda. Acción 2: girar a la derecha.
        long long giros[2] = {(d + 3) % 4, (d + 1) % 4};
        for (long long nueva_dir : giros) {
            long long v = codificar_estado_robot(f, c, nueva_dir, columnas);
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                cola[fin++] = v;
            }
        }

        // Acción 3: avanzar (la casilla debe existir y ser libre). Primero límites, luego pared.
        long long nf = f + df_dir[d], nc = c + dc_dir[d];
        if (dentro_de_grilla(nf, nc, filas, columnas) && grilla[nf][nc] != '#') {
            long long v = codificar_estado_robot(nf, nc, d, columnas);
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                cola[fin++] = v;
            }
        }
    }

    long long respuesta = INF;                       // mejor de las 4 direcciones en la meta
    for (long long d = 0; d < 4; d++)
        respuesta = min(respuesta, dist[codificar_estado_robot(filas - 1, columnas - 1, d, columnas)]);
    return respuesta == INF ? -1 : respuesta;
}

void resolver_robot_que_gira() {
    long long n, m;
    cin >> n >> m;
    vector<string> grilla = leer_grilla(n);
    cout << bfs_robot_que_gira(grilla) << "\n";
}

// =====================================================================================
//  SECCIÓN 10: PROBLEMA E - ROMPIENDO PAREDES (BFS 0-1 en grilla)
// =====================================================================================

// Mínimo de paredes a romper de (0,0) a (n-1,m-1). Siempre hay respuesta (nunca -1).
long long menor_cantidad_de_paredes(const vector<string> &grilla) {
    vector<vector<long long>> dist;
    bfs_01_grilla(grilla, {0, 0}, dist);
    return dist[grilla.size() - 1][grilla[0].size() - 1];
}

void resolver_rompiendo_paredes() {
    long long n, m;
    cin >> n >> m;
    vector<string> grilla = leer_grilla(n);
    cout << menor_cantidad_de_paredes(grilla) << "\n";
}

// =====================================================================================
//  SECCIÓN 11: PROBLEMA F - LUCES APAGADAS (estados como máscara de bits)
// =====================================================================================

// Convierte el tablero en una máscara: el bit (f * m + c) vale 1 si esa lámpara está encendida.
long long crear_mascara_luces(const vector<string> &tablero) {
    long long columnas = tablero[0].size(), mascara = 0;
    for (long long f = 0; f < (long long)tablero.size(); f++)
        for (long long c = 0; c < columnas; c++)
            if (tablero[f][c] == '1') mascara |= (1LL << (f * columnas + c));
    return mascara;
}

// efecto[i] = máscara de las lámparas que cambian al presionar la lámpara i (ella y sus vecinas).
vector<long long> calcular_efectos_luces(long long filas, long long columnas) {
    vector<long long> efecto(filas * columnas, 0);
    for (long long f = 0; f < filas; f++) {
        for (long long c = 0; c < columnas; c++) {
            long long i = f * columnas + c;
            efecto[i] |= (1LL << i);                        // la propia lámpara
            for (int k = 0; k < 4; k++) {
                long long nf = f + DF[k], nc = c + DC[k];
                if (dentro_de_grilla(nf, nc, filas, columnas))   // solo las vecinas que existen
                    efecto[i] |= (1LL << (nf * columnas + nc));
            }
        }
    }
    return efecto;
}

// BFS sobre los 2^(n*m) estados; presionar la lámpara i lleva de u a (u XOR efecto[i]).
// Meta = estado 0 (todas apagadas). Devuelve -1 si no se alcanza.
long long bfs_luces_apagadas(long long filas, long long columnas, long long estado_inicial) {
    long long cantidad = filas * columnas;
    vector<long long> efecto = calcular_efectos_luces(filas, columnas);

    vector<long long> dist(1LL << cantidad, INF);
    queue<long long> q;
    dist[estado_inicial] = 0;
    q.push(estado_inicial);

    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        for (long long i = 0; i < cantidad; i++) {   // presionar la lámpara i
            long long v = u ^ efecto[i];             // vecino generado al vuelo
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist[0] == INF ? -1 : dist[0];
}

void resolver_luces_apagadas() {
    long long n, m;
    cin >> n >> m;
    vector<string> tablero = leer_grilla(n);
    cout << bfs_luces_apagadas(n, m, crear_mascara_luces(tablero)) << "\n";
}

// =====================================================================================
//  SECCIÓN 12: PROBLEMA G - ROMPECABEZAS DESLIZANTE 2 x 3 (estados como string)
// =====================================================================================

// Posiciones del tablero (índices del string):   0 1 2
//                                                3 4 5
// Devuelve las posiciones vecinas (arriba/abajo/izquierda/derecha) de la posición p.
vector<long long> posiciones_vecinas_del_hueco(long long p) {
    long long fila = p / 3, columna = p % 3;
    vector<long long> vecinas;
    if (fila > 0) vecinas.push_back(p - 3);     // arriba
    if (fila < 1) vecinas.push_back(p + 3);     // abajo
    if (columna > 0) vecinas.push_back(p - 1);  // izquierda
    if (columna < 2) vecinas.push_back(p + 1);  // derecha
    return vecinas;
}

// Menor cantidad de movimientos hasta la meta "123450" (0 = hueco), o -1 si es imposible.
long long bfs_rompecabezas(const string &inicio, const string &meta) {
    map<string, long long> dist;                // si un estado no está, no fue descubierto
    queue<string> q;
    dist[inicio] = 0;
    q.push(inicio);

    while (!q.empty()) {
        string u = q.front();
        q.pop();
        if (u == meta) return dist[u];          // BFS: la primera vez que sale es la mínima
        long long hueco = u.find('0');
        for (long long pos : posiciones_vecinas_del_hueco(hueco)) {
            string v = u;
            swap(v[hueco], v[pos]);             // la ficha se desliza hacia el hueco
            if (!dist.count(v)) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return -1;
}

void resolver_rompecabezas_deslizante() {
    string inicio;
    for (int i = 0; i < 6; i++) {
        long long x;
        cin >> x;
        inicio += char('0' + x);
    }
    cout << bfs_rompecabezas(inicio, "123450") << "\n";
}

// =====================================================================================
//  SECCIÓN 13: DEMOSTRACIONES (datos de las diapositivas; así se ejecuta cada función)
// =====================================================================================

// El grafo de 8 vértices de la clase (capas {1},{2,3},{4,5},{6,8},{7} desde s = 1).
Grafo crear_grafo_de_la_clase() {
    Grafo lista_adyacencia = crear_grafo(8);
    long long aristas[][2] = {{1, 2}, {1, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 6}, {5, 6}, {5, 8}, {6, 7}};
    for (auto &arista : aristas) agregar_arista(lista_adyacencia, arista[0], arista[1], false);
    return lista_adyacencia;
}

void demostrar_bfs_en_grafos() {
    cout << "===== BFS en grafos (Algoritmos 1, 6, 8 y 10) =====\n";
    Grafo lista_adyacencia = crear_grafo_de_la_clase();
    vector<long long> dist, padre, cnt;

    bfs_basico(lista_adyacencia, 1, dist, padre);
    imprimir_vector("dist  desde 1 (vértices 1..8):", dist, 1);
    imprimir_vector("padre desde 1 (-1 = nulo)    :", padre, 1);
    imprimir_camino("camino de 1 a 7:", reconstruir_camino(7, dist, padre));

    vector<long long> orden = bfs_cola_arreglo(lista_adyacencia, 1, dist, padre);
    imprimir_vector("orden de visita (cola como arreglo):", orden, 0);

    contar_caminos_minimos(lista_adyacencia, 1, dist, cnt, 0);
    imprimir_vector("cnt (caminos mínimos desde 1):", cnt, 1);
    cout << "cnt[6] = " << cnt[6] << " (a 6 se llega por 4 con dos caminos y por 5 con uno)\n\n";
}

void demostrar_marcar_al_sacar() {
    cout << "===== Error clásico: marcar al sacar (Algoritmo 3) =====\n";
    Grafo triangulo = crear_grafo(3);              // s = 1, a = 2, b = 3
    agregar_arista(triangulo, 1, 2, false);
    agregar_arista(triangulo, 1, 3, false);
    agregar_arista(triangulo, 2, 3, false);
    vector<long long> dist, padre;
    long long entradas = 0;

    bfs_marcando_al_sacar_con_error(triangulo, 1, dist, entradas);
    cout << "marcando al sacar: dist[b] = " << dist[3] << " (INCORRECTO, la real es 1)\n";
    bfs_basico(triangulo, 1, dist, padre);
    cout << "marcando al meter: dist[b] = " << dist[3] << " (correcto)\n";

    Grafo k8 = crear_grafo(8);                     // grafo completo K8
    for (long long u = 1; u <= 8; u++)
        for (long long v = u + 1; v <= 8; v++) agregar_arista(k8, u, v, false);
    bfs_marcando_al_sacar_con_error(k8, 1, dist, entradas);
    cout << "en K8, marcando al sacar entran " << entradas << " elementos a la cola (en vez de 8)\n\n";
}

void demostrar_bfs_en_grilla() {
    cout << "===== BFS en grilla (Algoritmo 12) =====\n";
    vector<string> laberinto = {"S.#..", ".##.#", ".....", "#.#.#", "....T"};
    vector<vector<long long>> dist, cnt;
    vector<vector<Casilla>> padre;

    bfs_grilla(laberinto, {0, 0}, dist, padre);
    cout << "dist[T] = " << texto_distancia(dist[4][4]) << "\n";
    imprimir_camino_grilla("camino de S a T:", reconstruir_camino_grilla({4, 4}, dist, padre));

    contar_caminos_grilla(laberinto, {0, 0}, dist, cnt, 0);
    cout << "caminos mínimos de S a T: " << cnt[4][4] << "\n\n";
}

void demostrar_jarras() {
    cout << "===== Grafo de estados: jarras de 3 y 5 litros (Algoritmo 15) =====\n";
    cout << "vecinos de (0,0):";
    for (Casilla v : vecinos_jarras(0, 0, 3, 5)) cout << " (" << v.first << "," << v.second << ")";
    cout << "\n";
    vector<Casilla> camino;
    long long jugadas = bfs_jarras(3, 5, 4, camino);
    cout << "jugadas mínimas para tener 4 litros: " << jugadas << "\n";
    imprimir_camino_grilla("estados:", camino);
    cout << "\n";
}

void demostrar_escapar_del_fuego() {
    cout << "===== Escapar del fuego (dos BFS) =====\n";
    vector<string> grilla = {"#.###", "#...#", "#.#.#", "#P.F#", "##.##"};
    vector<vector<long long>> fuego = bfs_fuego(grilla, {{3, 3}});
    cout << "minuto en que arde la salida de abajo (4,2): " << fuego[4][2]
         << " | la de arriba (0,1): " << fuego[0][1] << "\n";
    cout << "minutos mínimos para escapar: " << resolver_escapar_del_fuego(grilla)
         << " (esperado 4: la salida de arriba)\n\n";
}

void demostrar_bfs_01() {
    cout << "===== BFS 0-1 (Algoritmo 17) =====\n";
    GrafoPonderado lista_adyacencia(7);
    long long aristas[][3] = {{1, 2, 1}, {1, 3, 0}, {2, 3, 0}, {3, 5, 1}, {2, 4, 1}, {4, 5, 0}, {5, 6, 1}};
    for (auto &a : aristas) {
        lista_adyacencia[a[0]].push_back({a[1], a[2]});
        lista_adyacencia[a[1]].push_back({a[0], a[2]});
    }
    vector<long long> dist;
    bfs_01(lista_adyacencia, 1, dist);
    imprimir_vector("dist desde 1 (esperado 0 0 0 1 1 2):", dist, 1);

    vector<string> terreno = {".#.", "###", ".#."};   // ejemplo 3 del problema E
    cout << "paredes a romper en la cruz de paredes: " << menor_cantidad_de_paredes(terreno)
         << " (esperado 2)\n\n";
}

void demostrar_usos_del_pdf_de_teoria() {
    cout << "===== Usos de BFS del PDF de teoría =====\n";
    // Varias fuentes: 8 pueblos, hospitales en 2 y 7.
    Grafo pueblos = crear_grafo(8);
    long long caminos[][2] = {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {3, 8}};
    for (auto &c : caminos) agregar_arista(pueblos, c[0], c[1], false);
    vector<long long> dist;
    bfs_varias_fuentes(pueblos, {2, 7}, dist);
    imprimir_vector("distancia al hospital más cercano (esperado 1 0 1 2 2 1 0 2):", dist, 1);

    // Componentes conexas y bipartito (grafo de la semana 11: tiene el triángulo 1-2-3).
    Grafo g11 = crear_grafo(8);
    long long aristas11[][2] = {{1, 2}, {1, 3}, {2, 3}, {2, 4}, {4, 5}, {6, 7}};
    for (auto &a : aristas11) agregar_arista(g11, a[0], a[1], false);
    vector<long long> componente, color;
    long long cantidad = etiquetar_componentes_conexas(g11, componente);
    imprimir_vector("componente de cada vértice:", componente, 1);
    cout << "cantidad de componentes: " << cantidad << " (esperado 3)\n";
    cout << "¿es bipartito? " << (es_bipartito(g11, color) ? "si" : "no") << " (esperado no: triángulo)\n";

    // Capa por capa en el grafo de la clase.
    vector<vector<long long>> capas = bfs_por_capas(crear_grafo_de_la_clase(), 1);
    for (size_t k = 0; k < capas.size(); k++) {
        cout << "capa " << k << ":";
        for (long long v : capas[k]) cout << " " << v;
        cout << "\n";
    }

    // Distancias HACIA el vértice 1 en el grafo dirigido de la semana 11.
    Grafo dirigido = crear_grafo(4);
    long long flechas[][2] = {{1, 2}, {1, 3}, {2, 4}, {3, 2}, {4, 1}};
    for (auto &f : flechas) agregar_arista(dirigido, f[0], f[1], true);
    distancias_hacia_vertice(dirigido, 1, dist);
    imprimir_vector("distancia de cada vértice HACIA el 1 (esperado 0 2 3 1):", dist, 1);

    // Escalera de palabras.
    unordered_set<string> diccionario = {"CAMA", "CASA", "CAPA", "COSA", "COPA", "ROSA", "ROPA"};
    vector<string> escalera = escalera_de_palabras(diccionario, "CAMA", "ROPA");
    cout << "escalera de palabras CAMA -> ROPA:";
    for (const string &palabra : escalera) cout << " " << palabra;
    cout << "  (" << (long long)escalera.size() - 1 << " cambios)\n\n";
}

void demostrar_problemas_con_sus_ejemplos() {
    cout << "===== Problemas C, D, E, F y G con los ejemplos de sus enunciados =====\n";
    // C
    Grafo c1 = crear_grafo(7);
    long long a1[][2] = {{1, 2}, {2, 3}, {3, 4}, {5, 6}};
    for (auto &a : a1) agregar_arista(c1, a[0], a[1], false);
    Grafo c2 = crear_grafo(5);
    long long a2[][2] = {{1, 2}, {2, 3}, {3, 1}, {4, 5}};
    for (auto &a : a2) agregar_arista(c2, a[0], a[1], false);
    cout << "C ejemplo 1: " << decidir_equipos_rivales(c1) << " (esperado SI 8)\n";
    cout << "C ejemplo 2: " << decidir_equipos_rivales(c2) << " (esperado NO)\n";
    // D
    cout << "D ejemplo 1: " << bfs_robot_que_gira({"..."}) << " (esperado 2)\n";
    cout << "D ejemplo 2: " << bfs_robot_que_gira({".#", ".."}) << " (esperado 4)\n";
    // E
    cout << "E ejemplo 2: " << menor_cantidad_de_paredes({"..#..", "###.#", "..#..", ".###.", "....."})
         << " (esperado 1)\n";
    // F
    cout << "F ejemplo 1: " << bfs_luces_apagadas(3, 3, crear_mascara_luces({"010", "111", "010"}))
         << " (esperado 1)\n";
    cout << "F ejemplo 2: " << bfs_luces_apagadas(1, 2, crear_mascara_luces({"10"})) << " (esperado -1)\n";
    // G
    cout << "G ejemplo 1: " << bfs_rompecabezas("123405", "123450") << " (esperado 1)\n";
    cout << "G ejemplo 2: " << bfs_rompecabezas("213450", "123450") << " (esperado -1)\n\n";
}

void ejecutar_todas_las_demostraciones() {
    demostrar_bfs_en_grafos();
    demostrar_marcar_al_sacar();
    demostrar_bfs_en_grilla();
    demostrar_jarras();
    demostrar_escapar_del_fuego();
    demostrar_bfs_01();
    demostrar_usos_del_pdf_de_teoria();
    demostrar_problemas_con_sus_ejemplos();
}

// =====================================================================================
//  MAIN: elige qué ejecutar según la primera palabra de la entrada
// =====================================================================================
int main() {
    cin.tie(0)->sync_with_stdio(false);

    string modo;
    if (!(cin >> modo)) modo = "DEMO";   // sin entrada: se muestran las demostraciones

    if (modo == "DEMO") ejecutar_todas_las_demostraciones();
    else if (modo == "C") resolver_equipos_rivales();
    else if (modo == "D") resolver_robot_que_gira();
    else if (modo == "E") resolver_rompiendo_paredes();
    else if (modo == "F") resolver_luces_apagadas();
    else if (modo == "G") resolver_rompecabezas_deslizante();
    else cout << "Modo desconocido. Usa DEMO, C, D, E, F o G.\n";

    return 0;
}
