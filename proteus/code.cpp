#include <Servo.h>
Servo servo;
// Configuration des broches pour les capteurs infrarouges
const int capteurGauche = 11;
const int capteurDroite = 12;

// Configuration des broches pour le L298N et le servo
const int IN1 = 7;
const int IN2 = 8;
const int IN3 = 9;
const int IN4 = 10;
const int ENB = 6;

const int ENA = 5;
const int trigPin = 2; // Broche TRIG du capteur ultrasonique
const int echoPin = 3;
int distance_L, distance_R;
void setup() {
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(capteurGauche, INPUT);
    pinMode(capteurDroite, INPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(ENA, OUTPUT);
    servo.attach(13);
}

void loop() {
    detecterObstacle();
    suivreLigne();
}

void detecterObstacle() {
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    float distanceCM = (duration * 0.034 / 2) / 10;
    if (distanceCM < 20) {
        arrete();
        servo.write(180); // Déplacer le servo vers la position 180 degré
        delay(100); // Reduce delay for faster simulation
        check_side();
    }
}
void suivreLigne() {
    int valeurGauche = digitalRead(capteurGauche);
    int valeurDroite = digitalRead(capteurDroite);

    if (valeurGauche == HIGH && valeurDroite == HIGH) {
        avancer();
    } else if (valeurGauche == HIGH) {
        tournerDroite();
    } else if (valeurDroite == HIGH) {
        tournerGauche();
    } else {
        arrete();
    }
}
void check_side() {
    for (int angle = 70; angle <= 140; angle += 5) {
        servo.write(angle);
        delay(50);
    }
    long distances[2];
    for (int i = 0; i < 2; i++) {
        int direction = (i == 0) ? HIGH : LOW;
        digitalWrite(IN1, direction);
        digitalWrite(IN2, !direction);
        digitalWrite(ENA, HIGH);
        digitalWrite(ENB, LOW);
        digitalWrite(IN3, direction);
        digitalWrite(IN4, !direction);
        delay(100); // Allow motor to spin and sensor to read
        distances[i] = pulseIn(echoPin, HIGH);
    }
    distance_L = ((distances[0] * 0.034 / 2) / 10);
    distance_R = ((distances[1] * 0.034 / 2) / 10);
    compareDistance();
}


void compareDistance() {
    if (distance_L > distance_R) {
        tournerGauche();
        delay(500); // Adjust delay based on motor speed
        avancer();
        delay(500);
        tournerDroite();
        delay(500);
    } else {
        tournerDroite();
        delay(500);
        avancer();
        delay(500);
        tournerGauche();
        delay(500);
    }
}


void avancer() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(ENA, HIGH);
    digitalWrite(ENB, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Le robot avance");
    delay(100);
}


void tournerGauche() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(ENA, LOW);
    digitalWrite(ENB, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Le robot tourne a gauche");

    delay(100);
}


void tournerDroite() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(ENA, HIGH);
    digitalWrite(ENB, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.println("Le robot tourne a droite");
    delay(100);
}


void arrete() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    digitalWrite(ENA, LOW);
    digitalWrite(ENB, LOW);
    Serial.println("Le robot est a l'arrêt");
    delay(100);
}


long Ultrasonic_read() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);

    delayMicroseconds(10);
    long duration = pulseIn(echoPin, HIGH);
    return ((duration * 0.034 / 2)/10);
}