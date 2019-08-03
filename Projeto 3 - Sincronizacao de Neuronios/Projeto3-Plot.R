# PROJETO 3 - MODELAGEM COMPUTACIONAL
# Prof. Dr. Marcos Gonçalves Quiles
# Sincronização de Rede de Neurônios Acoplados
# 
# Nomes: Ana Júlia de Oliveira Bellini			  RA: 111774
# Luiz Filipe Moraes Saldanha de Oliveira     RA: 112229
# Willian Dihanster Gomes de Oliveira         RA: 112269

t <- 2000
deltaT <- 0.01
n <- 500

setwd("/home-local/aluno/Área de Trabalho")

dados = read.csv("/home-local/aluno/Downloads/plotneuro.csv")

dados <- dados[1:length(dados[1,]) - 1]

vetor <- seq(0, 0, len = t)
selec <- c(1:length(dados[1,]))

cores <- c("#008B8B", "#778899", "#0000FF", "#1C1C1C", "#9400D3", "#FF0000", "#32CD32")

plot(vetor, ylim = c(-3,28), type = 'n', xlab = ' ', ylab = ' ', main = 'Rede regular (50% Ativos, sem acoplamento)')
for (i in selec)
  lines(dados[,i], col = cores[(i %% length(cores))])
  
  