Os vértices de um grafo são formados pelos jogadores e pelas celas descritas no livro. Com esses elementos, a matriz de adjacências tem dimensão n_celas+1 * n_jogadores+1, onde a primeira linha e a primeira coluna tem as celas decritas no livro e os jogadores participantes. Dessa forma, há aresta entre dois vértices (um jogador e uma cela) quando, subtraindo de todas as qualidades da cela maiores que as qualidades do jogador as qualidades do jogador e as somando, obtemos um número menor ou igual a dois. Assim, para que uma cela seja eleita uma solução do problema, percorremos a linha em que está e todas as posições dessa linha, de i=1 a i=jogadores+1, devem ser adjacentes a cela (ter 1 na matriz de adjacencia).