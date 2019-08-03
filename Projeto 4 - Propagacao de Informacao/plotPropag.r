#PROJETO 4 - MODELAGEM COMPUTACIONAL
#Prof. Dr. Marcos Gonçalves Quiles
#Propagação de Informação - Consenso

#Nomes: Ana Júlia de Oliveira Bellini			  RA: 111774
#	   Luiz Filipe Moraes Saldanha de Oliveira    RA: 112229
#	   Willian Dihanster Gomes de Oliveira        RA: 112269

#setwd("/home-local/aluno/Pasta sem título")

dados = read.csv("plotPropag.txt", header = FALSE)

cores = c("#FFFF00", "#FF8000", "#A9F5F2", "#008B8B", "#778899", "#0000FF", "#1C1C1C",
          "#9400D3", "#FF0000", "#32CD32")

plot((1:1), (1:1), xlim = c(1,(length(dados[,1]))),
     ylim = c(0,((2.5/2) * max(dados[length(dados[,1]),]))), type = 'h',
     xlab = "Iterações", ylab = "Opiniões",
     main = "Propagação de Informação")

for(i in (1: length(dados[1,])))
  lines(dados[,i], col = cores[i])

legend("topleft", legend = c(1:length(dados[1,])), cex = 0.6, ncol = length(dados[1,])/2, lty = 1,
         col = cores, lwd = 2, bty = "n")
