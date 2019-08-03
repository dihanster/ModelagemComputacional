#Parametros
g = -9.81
m = 5
k = 0.5
deltaT = 0.01
theta = 0.9
tempo = 0

#Definindo Vetores
vetX = 1:1
vetY = 1:1
vetSx = 1:1
vetSy = 1:1
vetAbs = 1:1
Ec = 1:1
Ep = 1:1
Em = 1:1

#Iniciando Vetores
vetAbs[1] = 10
vetX[1] = vetAbs[1]*cos(theta)
vetY[1] = vetAbs[1]*sin(theta)
vetSx[1] = 0
vetSy[1] = 0

Sy = vetSy[1]

Ec[1]= (((vetAbs[1])^2) * m)/2
Ep[1] = m * (-g) * vetSy[1]
Em[1] = Ec[1] + Ep[1]

i = 2

while(Sy > 0 || i == 2){
    vX =  vetX[i-1] + ((- k * vetX[i-1])/m) * deltaT
    vetX <- append(vetX, vX)
    
    vY = vetY[i-1] + (g - (k/m)*vetY[i-1]) * deltaT
    vetY <- append(vetY, vY)
    
    Sx = vetSx[i-1] + vetX[i]*deltaT
    vetSx <- append(vetSx, Sx)
    
    Sy = vetSy[i-1] + vetY[i]*deltaT
    vetSy <- append(vetSy, Sy)
    
    vAbs = sqrt((vetX[i])^2 + (vetY[i])^2)
    vetAbs <- append(vetAbs, vAbs)
    
    K = (((vetAbs[i])^2) * m)/2
    Ec <- append(Ec, K)
    
    U = m * (-g) * vetSy[i]
    Ep <- append(Ep, U)
    
    E = Ec[i] + Ep[i]
    Em <- append(Em, E)
    
    tempo = i
    
    i = i+1
}

tempo = tempo * deltaT

plot(vetY, xlab = "Tempo (s x 10⁻²)", ylab = "Vy (m)")

plot(vetSy, xlab = "Tempo (s x 10⁻²)", ylab = "Sy (m)")

plot(vetX, xlab = "Tempo (s x 10⁻²)", ylab = "Vx (m)")

plot(vetSx, xlab = "Tempo (s x 10⁻²)", ylab = "Sx (m)")

plot(vetAbs, xlab = "Tempo (s x 10⁻²)", ylab = "V (m/s)")

plot(Ec, xlab = "Tempo (s x 10⁻²)", ylab = "Ec (J)")

plot(Ep, xlab = "Tempo (s x 10⁻²)", ylab = "Ep (J)")

plot(Em, xlab = "Tempo (s x 10⁻²)", ylab = "Em (J)")

plot(vetSx,vetSy, xlab = "Sx (m)", ylab = "Sy (m)")

