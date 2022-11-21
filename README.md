# Máquina de Turing Reversível

## Autores

- Bento Borges Schirmer.
- Fernando Kalikosque Laydner Júnior.
- Miguel Antônio Welter Wisneski.

## Compilação

Em ambientes UNIX:

    $ make

Em ambiente Windows:

    > compila.bat

## Execução

    ./mtr [45R] < exemplo-*.txt

## Exemplos

 1. Decisor da expressão regular (a + b)\*, testando 0011.
 2. Decisor da expressão regular 0\*, testando 000.
 3. Decisor da linguagem {a<sup>n</sup>b<sup>n</sup> | n >= 1}, testando aabb.
 4. Decisor da linguagem {a<sup>n</sup>b<sup>n</sup> | n >= 1}, testando aab.
 5. Decisor da linguagem {a<sup>n</sup>b<sup>n</sup>c<sup>n</sup> | n >= 1},
    testando aabbcc.
 6. Somador de x + y, testando 111110111.
 7. Comparador de ou x >= y ou x < y, testando 111110111.
 8. Comparador de ou x >= y ou x < y, testando 110111.
 9. Comparador que soma x + y se x >= y, senão gera 0, testando 111110111.
10. Comparador que soma x + y se x >= y, senão gera 0, testando 110111.
