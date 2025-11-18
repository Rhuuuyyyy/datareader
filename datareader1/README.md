# Leitor de Dados em C
Esse repositório foi criado para o desenvolvimento do trabalho de Programação Imperativa | PUC-PR
A finalidade do programa consiste em ler a analisar dados para o usuário.

_Com base nessa tabela: https://www.cfn.org.br/wp-content/uploads/2017/03/taco_4_edicao_ampliada_e_revisada.pdf
(páginas 24 à 65)._

## Categorias de alimentos:
1. Cereais e derivados
2. Verduras, hortaliças e derivados
3. Frutas e derivados
4. Gorduras e óleos
5. Pescados e frutos do mar
6. Carnes e derivados
7. Leite e derivados
8. Bebidas (alcoólicas e não alcoólicas)
9. Ovos e derivados
10. Produtos açucarados
11. Miscelâneas
12. Outros alimentos industrializados
13. Alimentos preparados
14. Leguminosas e derivados
15. Nozes e sementes
 
### 1. Função
Essa função deve selecionar aleatoriamente 100 alimentos da Tabela 1;
Com no mínimo, 10 categorias e no mínimo 5;
No máximo, 20 alimentos de cada categoria selecionada;
Os dados retornados devem ser transcritos para um arquivo texto de formato livre ou em um arquivo csv com essas informações:
• Número do alimento
• Descrição do alimento
• Umidade (%)
• Energia (Kcal)
• Proteína (g)
• Carboidrato (g)
• Categoria do alimento.
 

### 2. Função 
A função deve ler os dados sobre alimentos do arquivo construído na 1. Função e armazenar esses dados em memória, usando um vetor (cada elemento do vetor
deve conter todos os dados de um alimento).
A função deve apresentar as seguintes opções ao usuário:

a. Liste todas as categorias de alimentos.

b. Liste todos os alimentos de certa categoria selecionada pelo usuário,
em ordem alfabética de acordo com a descrição do alimento.

c. Liste todos os alimentos de certa categoria selecionada pelo usuário,
em ordem decrescente com respeito à capacidade energética dos
alimentos.

d. Listar os N alimentos de certa categoria com maior percentual de
umidade, em ordem decrescente com respeito a esse percentual,
sendo a categoria e N definidos pelo usuário.

e. Listar os N alimentos de certa categoria com maior capacidade
energética, em ordem decrescente com respeito a essa capacidade,
sendo a categoria e N definidos pelo usuário.

f. Listar os N alimentos de certa categoria com maior quantidade de
proteína, em ordem decrescente com respeito a essa quantidade,
sendo a categoria e N definidos pelo usuário.

g. Listar os N alimentos de certa categoria com maior quantidade de
carboidrato, em ordem decrescente com respeito a essa quantidade,
sendo a categoria e N definidos pelo usuário.

h. Listar os N alimentos de certa categoria que possuam a relação mais
alta entre energia e proteína, em ordem decrescente com respeito a
essa relação, sendo a categoria e N definidos pelo usuário.

i. Listar os N alimentos de certa categoria que possuam a relação mais
alta entre energia e carboidrato, em ordem decrescente com respeito a
essa relação, sendo a categoria e N definidos pelo usuário.

j. Encerrar o programa.

# Requisitos de implementação:
1. O vetor que contém os dados sobre alimentos não pode ser modificado durante a
execução do programa.
2. A representação das categorias de alimentos deve ser implementada por meio de
um enumerado.
3. A implementação da ordenação deve ser feita de forma eficiente com respeito ao
uso de memória. Assim, por exemplo, para se obter os N alimentos com maior
percentual de umidade, não se deve replicar todo o vetor de dados e, então,
ordená-lo; o único dado que precisa ser replicado para proceder a ordenação é a
própria porcentagem de umidade de cada alimento. No entanto, é preciso
associar um ponteiro para cada valor de porcentagem de umidade, isto é, um
ponteiro para o correspondente alimento no vetor a fim de listar os seus dados
completos ao emitir a resposta para o usuário.
4. Pode ser empregado qualquer algoritmo de ordenação disponível na literatura, tal
como o Bubble Sort. Mas, é o programa deve realmente implementar o algoritmo,
isto é, não pode simplesmente usar uma função de biblioteca que faça a
ordenação.
5. Não é necessário estruturar o programa em funções, mas convém que seja feito
dessa forma a fim de facilitar o desenvolvimento do programa, bem como a sua
compreensão.

