############################################################
# VARIÁVEIS E PARÂMETROS
i = 1
t = 50000 # tempo
k = 200 # capacidade do ambiente
deltaT = 0.01
vet <- seq(0, t * 0.01, deltaT)

# variáveis da vegetação
alfa0 = 10
alfa1 = 0.1
alfa2 = 0.1
alfa3 = 0.1

# variáveis das capivaras
beta0 = 1.75
beta1 = 0.175
beta2 = 0.09

# variáveis dos ratos
gamma0 = 0.8
gamma1 = 0.15
gamma2 = 0.09
gamma3 = 0.09

# variáveis dos insetos
sigma0 = 2.25
sigma1 = 0.2
sigma2 = 0.09

# variáveis das onças
lambda0 = 8.5
lambda1 = 0.1
lambda2 = 0.1

# variáveis das corujas
theta0 = 8.5
theta1 = 0.1
theta2 = 0.1

# populações iniciais
vegAux = 15
capAux = 5
ratoAux = 70
insAux = 25
onAux = 15
corAux = 5

veg = 1:1
cap = 1:1
rato = 1:1
ins = 1:1
on = 1:1 
cor = 1:1

veg[1] = vegAux 
cap[1] = capAux
rato[1] = ratoAux
ins[1] = insAux
on[1] = onAux
cor[1] = corAux

############################################################
# MODELAGEM
while (i <= t) {
  i = i + 1
  
  ### VEGETAÇÃO
  vegAux = veg[i-1] + veg[i-1] * (alfa0 * (1 - (veg[i-1])/k) - alfa1 * cap[i-1] - alfa2 * rato[i-1] - alfa3 * ins[i-1]) * deltaT
  
  if(vegAux < 0) {
    vegAux = 0
  }
  
  # PERTURBAÇÃO 1: QUEIMADA
  # if (i > 30000 && i <= 30250) {
  #   vegAux = vegAux - vegAux * 0.1
  # }
  
  veg = append(veg, vegAux) 
  
  ### CAPIVARAS
  capAux = cap[i-1] + cap[i-1] * (beta1 * veg[i-1] - beta2 * on[i-1] - beta0) * deltaT
  
  if(capAux < 0) {
    capAux = 0
  }
  
  cap = append(cap, capAux)
  
  ### RATOS
  ratoAux = rato[i-1] + rato[i-1] * (gamma1 * veg[i-1] - gamma2 * on[i-1] - gamma3 * cor[i-1] - gamma0) * deltaT
  
  if(ratoAux < 0) {
    ratoAux = 0
  }
  
  # PERTURBAÇÃO 2: SURTO DOS RATOS
  # if (i >= 30000 && i < 30100) {
  #   ratoAux = ratoAux * 1.05
  # }
  
  rato = append(rato, ratoAux)
  
  ### INSETOS
  insAux = ins[i-1] + ins[i-1] * (sigma1 * veg[i-1] - sigma2 * cor[i-1] - sigma0) * deltaT
  
  if(insAux < 0) {
    insAux = 0
  }
  
  ins = append(ins, insAux)
  
  ### ONÇAS
  onAux = on[i-1] + on[i-1] * (lambda1 * cap[i-1] + lambda2 * rato[i-1] - lambda0) * deltaT
  
  if(onAux < 0) {
    onAux = 0
  }
  
  # PERTURBAÇÃO 3: CAÇA ÀS ONÇAS
  # if (i == 30000) {
  #   onAux = onAux - 5
  # }
  
  on = append(on, onAux)   
  
  ### CORUJAS
  corAux = cor[i-1] + cor[i-1] * (theta1 * rato[i-1] + theta2 * ins[i-1] - theta0) * deltaT
  
  if (corAux < 0) {
    corAux = 0
  }
  
  # PERTURBAÇÃO 4: EXTINÇÃO DAS CORUJAS
  # if (i == 30000) {
  #   corAux = 0
  # }
  
  cor = append(cor, corAux)  
}

############################################################
# GRÁFICOS DE CADA INDIVÍDUO
plot(vet, cor, type="l", pch = 1, lwd = 2, col = "#008B8B", xlab = "Tempo", ylab = "Corujas", main = "Corujas x Tempo")
plot(vet, on, type="l", pch = 1, lwd = 2, col = "#008B8B", xlab = "Tempo", ylab = "Onças", main = "Onças x Tempo")
plot(vet, ins, type="l", pch = 1, lwd = 2, col = "#008B8B", xlab = "Tempo", ylab = "Insetos", main = "Insetos x Tempo")
plot(vet, rato, type="l", pch = 1, lwd = 2, col = "#008B8B", xlab = "Tempo", ylab = "Ratos", main = "Ratos x Tempo")
plot(vet, cap, type="l", pch = 1, lwd = 2, col = "#008B8B", xlab = "Tempo", ylab = "Capivaras", main = "Capivaras x Tempo")
plot(vet, veg, type="l", pch = 1, lwd = 2, col = "#008B8B", xlab = "Tempo", ylab = "Vegetação", main = "Vegetação x Tempo")

############################################################
# GRÁFICOS LOTKA-VOLTERRA
plot(rato, cor,  type="l", pch = 2, lwd = 2, col = "#008B8B", xlab = "Ratos", ylab = "Corujas", main = "Corujas x Ratos") 
plot(ins, cor ,  type="l", pch = 1, lwd = 2, col = "#008B8B", xlab = "Insetos", ylab = "Corujas", main = "Corujas x Insetos") 
plot(veg, ins ,  type="l", pch = 1, lwd = 2, col = "#008B8B", xlab = "Vegetação", ylab = "Insetos", main = "Insetos x Vegetação") 
plot(veg, rato ,  type="l", pch = 1, lwd = 2, col = "#008B8B",xlab = "Vegetação", ylab = "Ratos", main = "Ratos x Vegetação") 
plot(veg, cap ,  type="l", pch = 1, lwd = 2, col = "#008B8B", xlab = "Vegetação", ylab = "Capivaras", main = "Capivaras x Vegetação") 
plot(cap, on ,  type="l", pch = 1, lwd = 2, col = "#008B8B", xlab = "Capivaras", ylab = "Onças", main = "Onças x Capivaras") 
plot(rato, on,  type="l", pch = 1, lwd = 2, col = "#008B8B", xlab = "Ratos", ylab = "Onças", main = "Onças x Ratos") 

############################################################
# GRÁFICOS 2 A 2
plot(vet, cor, type="l", pch = 1, lwd = 2, col = "#61210B", ylim = c(0, max(ins)), xlab = "Tempo", ylab = "Populações", main = "População de Insetos e Corujas")
lines(vet, ins, type="l", pch = 1, lwd = 2, col = "#5FB404")
legend("topright", legend = c("Corujas", "Insetos"), 
       cex = 0.75, ncol=2, lty=1, col = c("#61210B", "#5FB404"), lwd=2, bty="n")

plot(vet, veg, type="l", pch = 1, lwd = 2, col = "#0B610B", ylim = c(0, max(rato) + 10), xlab = "Tempo", ylab = "Populações", main = "População de Ratos e Vegetação")
lines(vet, rato, type="l", pch = 1, lwd = 2, col = "#FF0000")
legend("topright", legend = c("Vegetação", "Ratos"), 
       cex = 0.75, ncol=2, lty=1, col = c("#0B610B", "#FF0000"), lwd=2, bty="n")

plot(vet, on, type="l", pch = 1, lwd = 2, col = "#FFBF00", ylim = c(0, max(cap) + 5), xlab = "Tempo", ylab = "Populações", main = "População de Onças e Capivaras")
lines(vet, cap, type="l", pch = 1, lwd = 2, col = "#8A4B08")
legend("topright", legend = c("Onças", "Capivaras"), 
       cex = 0.75, ncol=2, lty=1, col = c("#FFBF00", "#8A4B08"), lwd=2, bty="n")
