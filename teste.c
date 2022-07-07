// Portas driver motor
#define PININ1 2
#define PININ2 4
#define PININ3 5
#define PININ4 7
#define PINENA 3
#define PINENB 6

// Portas led rgb
#define PINLEDR 11
#define PINLEDG 9
#define PINLEDB 10

// Portas sensor QTR
#define SENSOR1 A0
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3
#define SENSOR5 A4
#define SENSOR6 A5

// Valores de ajustes para o seguidor de linha MIF
#define TRESHOLD 700 // Valor de referencia para cor da linha branca
#define SPEED0 255   // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 1 1 0 0)
#define SPEED1 210   // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 1 1 1 0)

#define SPEED2 180 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 1 0 0)
#define SPEED3 150 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 1 1 0)
#define SPEED4 100 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 1 1 1)

#define SPEED5 0   // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 0 1 0)
#define SPEED6 0   // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 0 1 1)
#define SPEED7 100 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 0 0 1)

#define RUNTIME 35000 // Valor para executar o percurso

double Kp = 20, Ki = 0, Kd = 0.5, P, I, D, PIDvalue, error, pastError;

void setup()
{

    rgbControl(255, 0, 0, 1000);
    rgbControl(0, 255, 0, 1000);
    rgbControl(0, 0, 255, 1000);
}

void loop()
{
    followLineMEF();
}

void advancedOption(char option)
{
    // Função para controlar de opçãoes avançadas
    switch (option)
    {
    case 'a':
        readSensors();
        break;
    case 'b':
        followLineMEF(); // Seguidor de linha com Máquina de Estados Finitos
        break;
    }
}

void motorControl(int speedLeft, int speedRight)
{
    // Função para controle do driver de motor

    // Definições das portas digitais
    pinMode(PININ1, OUTPUT);
    pinMode(PININ2, OUTPUT);
    pinMode(PININ3, OUTPUT);
    pinMode(PININ4, OUTPUT);
    pinMode(PINENA, OUTPUT);
    pinMode(PINENB, OUTPUT);

    // Ajustes motor da esquerda
    if (speedLeft < 0)
    {
        speedLeft = -speedLeft;
        digitalWrite(PININ3, HIGH);
        digitalWrite(PININ4, LOW);
    }
    else
    {
        digitalWrite(PININ3, LOW);
        digitalWrite(PININ4, HIGH);
    }

    // Ajustes motor da direita
    if (speedRight < 0)
    {
        speedRight = -speedRight;
        digitalWrite(PININ1, LOW);
        digitalWrite(PININ2, HIGH);
    }
    else
    {
        digitalWrite(PININ1, HIGH);
        digitalWrite(PININ2, LOW);
    }

    analogWrite(PINENA, speedLeft);
    analogWrite(PINENB, speedRight);
}

void motorOption(char option, int speedLeft, int speedRight)
{
    // Função para controle de motor com pre definições
    switch (option)
    {
    case '6': // Esquerda
        motorControl(-speedLeft, speedRight);
        break;
    case '4': // Direita
        motorControl(speedLeft, -speedRight);
        break;
    case '2': // Trás
        motorControl(-speedLeft, -speedRight);
        break;
    case '8': // Frente
        motorControl(speedLeft, speedRight);
        break;
    case '0': // Parar
        motorControl(0, 0);
        break;
    }
}

bool motorStop(long runtime, long currentTime)
{
    // Função de parada do robô
    if (millis() >= (runtime + currentTime))
    {
        motorOption('0', 0, 0);
        int cont = 0;
        while (cont < 5)
        {
            rgbControl(255, 0, 0, 500);
            rgbControl(0, 0, 0, 500);
            cont++;
        }
        return false;
    }
    return true;
}

void rgbControl(int red, int green, int blue, long rumtime)
{
    // Função para controle do led rgb
    pinMode(PINLEDR, OUTPUT);
    pinMode(PINLEDG, OUTPUT);
    pinMode(PINLEDB, OUTPUT);

    digitalWrite(PINLEDR, HIGH);
    digitalWrite(PINLEDG, HIGH);
    digitalWrite(PINLEDB, HIGH);

    analogWrite(PINLEDR, red);
    analogWrite(PINLEDG, green);
    analogWrite(PINLEDB, blue);
    delay(rumtime);
}

void readSensors(void)
{
}

void followLineMEF(void)
{
    // Função para controle do seguidor de linha em modo de maquina de estado finita
    bool flag = true;
    long currentTime = millis();

    while (flag)
    {

        P = error;
        I = I + error;
        D = error - pastError;
        PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);

        pastError = error;

        flag = motorStop(RUNTIME, currentTime);

        // leitura do sensor (1 1 1 1 1 1)
        if (analogRead(A0) <= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) <= TRESHOLD)
        {
            error = 0;
            motorOption('8', SPEED0 - PIDvalue, SPEED0 - PIDvalue);
            // leitura do sensor (0 1 1 1 1 0)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            motorOption('8', SPEED0 - PIDvalue, SPEED0 - PIDvalue);
            // leitura do sensor (0 0 1 1 0 0)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = 0;
            motorOption('8', SPEED0 - PIDvalue, SPEED0 - PIDvalue);

            // leitura do sensor (0 1 1 1 0 0)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = -1;
            motorOption('8', SPEED0 - PIDvalue, SPEED1 - PIDvalue);

            // leitura do sensor (0 0 1 1 1 0)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = 1;
            motorOption('8', SPEED1 - PIDvalue, SPEED0 - PIDvalue);

            // leitura do sensor (0 0 1 0 0 0)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = -1;
            motorOption('8', SPEED0 - PIDvalue, SPEED2 - PIDvalue);
            // leitura do sensor (0 0 0 1 0 0)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = 1;
            motorOption('8', SPEED2 - PIDvalue, SPEED0 - PIDvalue);

            // leitura do sensor (0 1 1 0 0 0)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = -2;
            motorOption('8', SPEED0 - PIDvalue, SPEED3 - PIDvalue);

            // leitura do sensor (0 0 0 1 1 0)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = 2;
            motorOption('8', SPEED3 - PIDvalue, SPEED0 - PIDvalue);

            // leitura do sensor (1 1 1 0 0 0)
        }
        else if (analogRead(A0) <= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = -3;
            motorOption('8', SPEED0 - PIDvalue, SPEED4 - PIDvalue);
            // leitura do sensor (0 0 0 1 1 1)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) <= TRESHOLD)
        {
            error = 3;
            motorOption('8', SPEED4 - PIDvalue, SPEED0 - PIDvalue);

            // leitura do sensor (0 1 0 0 0 0)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = -2;
            motorOption('8', SPEED0 - PIDvalue, SPEED5 - PIDvalue);

            // leitura do sensor (0 0 0 0 1 0)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = 2;
            motorOption('8', SPEED5 - PIDvalue, SPEED0 - PIDvalue);

            // leitura do sensor (1 1 0 0 0 0)
        }
        else if (analogRead(A0) <= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = -4;
            motorOption('8', SPEED0 - PIDvalue, SPEED6 - PIDvalue);

            // leitura do sensor (0 0 0 0 1 1)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) <= TRESHOLD)
        {
            error = 4;
            motorOption('8', SPEED6 - PIDvalue, SPEED0 - PIDvalue);

            // leitura do sensor (1 0 0 0 0 0)
        }
        else if (analogRead(A0) <= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD)
        {
            error = -5;
            motorOption('6', SPEED7 - PIDvalue, SPEED7 - PIDvalue);
            // leitura do sensor (0 0 0 0 0 1)
        }
        else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) <= TRESHOLD)
        {
            error = 5;
            motorOption('4', SPEED7 - PIDvalue, SPEED7 - PIDvalue);
        }
    }
    motorOption('0', 0, 0);
}
