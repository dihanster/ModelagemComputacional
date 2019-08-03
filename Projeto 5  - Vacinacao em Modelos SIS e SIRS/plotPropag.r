#########################################################################
# PROJETO 5 - MODELAGEM COMPUTACIONAL                                   #
# Prof. Dr. Marcos Goncalves Quiles                                     #
# Propagacao de Informacaoo - Modelo SIRS                               #
#                                                                       #
# Nomes:  Ana Julia de Oliveira Bellini			        RA: 111774          #
#	        Luiz Filipe Moraes Saldanha de Oliveira   RA: 112229          #
#	        Willian Dihanster Gomes de Oliveira       RA: 112269          #
#########################################################################

dados = read.csv("plotPropag.txt", header = FALSE)
#y = c(1:100000)
#plot(dados, ylim = c(0, 10))
#plot(dados, y)
#lines(dados)

cores = c("#FFFF00", "#FF8000", "#A9F5F2", "#008B8B", "#778899", "#0000FF", "#1C1C1C",
          "#9400D3", "#FF0000", "#32CD32")

plot((1:1), (1:1), xlim = c(1,100),
     ylim = c(0, 1000), type = 'h',
     xlab = "Iteracoes", ylab = "Infectados",
     main = "Propagacao da Doenca")

lines(dados, col = cores[4], lwd = 3)

#legend("topleft", legend = c(1:length(dados[1,])), cex = 0.6, ncol = length(dados[1,])/2, lty = 1,
 #        col = cores, lwd = 2, bty = "n")
