#include <Servo.h>
#include <NewPing.h>

#define enA 12    // Broche de commande 1 du L298 : enA
#define in1 4       // Broche de moteur 1 du L298 : in1
#define in2 2       // Broche de moteur 1 du L298 : in1
#define in3 6      // Broche de moteur 2 du L298 : in1
#define in4 5      // Broche de moteur 2 du L298 : in1
#define enB 7      // Broche de commande 2 du L298 : enB
#define L_S 10      // Broche numérique pour le capteur infrarouge gauche
#define R_S 13       // Broche numérique pour le capteur infrarouge droit
#define echo 11      // Broche numérique pour le capteur ultrasonique Echo
#define trigger 8  // Broche numérique pour le capteur ultrasonique Trigger
#define servoPin 3    // Broche numérique pour le servomoteur

int Set = 15;
int distance_L, distance_F, distance_R;

Servo myServo; // Crée une instance de la classe Servo
NewPing sonar(trigger, echo); // Crée une instance de la classe NewPing

void setup() {
  Serial.begin(9600); // Démarre la communication série à 9600bps
  pinMode(R_S, INPUT); // Déclare le capteur droit comme entrée
  pinMode(L_S, INPUT); // Déclare le capteur gauche comme entrée
  pinMode(enA, OUTPUT); // Déclare la broche enA comme sortie pour le moteur 1
  pinMode(in1, OUTPUT); // Déclare la broche in1 comme sortie pour le moteur 1
  pinMode(in2, OUTPUT); // Déclare la broche in2 comme sortie pour le moteur 1
  pinMode(in3, OUTPUT); // Déclare la broche in3 comme sortie pour le moteur 2
  pinMode(in4, OUTPUT); // Déclare la broche in4 comme sortie pour le moteur 2
  pinMode(enB, OUTPUT); // Déclare la broche enB comme sortie pour le moteur 2
  myServo.attach(servoPin); // Attache le servomoteur à la broche
  analogWrite(enA, 150); // Écrit le cycle de service 0 à 255 sur la broche Enable A pour la vitesse du moteur 1
  analogWrite(enB, 150); // Écrit le cycle de service 0 à 255 sur la broche Enable B pour la vitesse du moteur 2
}

void loop() {
  // Serial.println("etat  capteur Droite : ");
  // Serial.println(digitalRead(R_S));
  // Serial.println("etat  capteur Gauche : ");
  // Serial.println(digitalRead(L_S));

  distance_F = sonar.ping_cm();
  Serial.print("D F="); Serial.println(distance_F);

  if ((digitalRead(R_S) == LOW) && (digitalRead(L_S) == LOW)) {
    if (distance_F > Set) {
      enAvant();
      Serial.print("Avancer le robot ");
    } else {
      verifierCote();
    }
  } else if ((digitalRead(R_S) == HIGH) && (digitalRead(L_S) == LOW)) {
    Serial.print("Tourner a droite le robot ");
    tournerDroite();
  } else if ((digitalRead(R_S) == LOW) && (digitalRead(L_S) == HIGH)) {
    Serial.print("Tourner a gauche le robot ");
    tournerGauche();
  } else {
    arreter();

  }

  delay(10);
} 


void impulsionServo(int angle) {
  myServo.write(angle);
  delay(50); // Cycle de rafraîchissement du servomoteur
}

void comparerDistances() {
  if (distance_L > distance_R) {
    tournerGauche();
    delay(500);
    enAvant();
    delay(600);
    tournerDroite();
    delay(500);
    enAvant();
    delay(600);
    tournerDroite();
    delay(400);
  } else {
    tournerDroite();
    delay(500);
    enAvant();
    delay(600);
    tournerGauche();
    delay(500);
    enAvant();
    delay(600);
    tournerGauche();
    delay(400);
  }
}

void verifierCote() {
  arreter();
  delay(100);
  for (int angle = 70; angle <= 140; angle += 5) {
    impulsionServo(angle);
  }
  delay(300);
  distance_R = sonar.ping_cm();
  Serial.print("D R="); Serial.println(distance_R);
  delay(100);
  for (int angle = 140; angle >= 0; angle -= 5) {
    impulsionServo(angle);
  }
  delay(500);
  distance_L = sonar.ping_cm();
  Serial.print("D L="); Serial.println(distance_L);
  delay(100);
  for (int angle = 0; angle <= 70; angle += 5) {
    impulsionServo(angle);
  }
  delay(300);
  comparerDistances();
}

void enAvant() {
  digitalWrite(in1, LOW);
  digitalWrite(in2,HIGH );
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void tournerDroite() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void tournerGauche() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void arreter() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
