#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define FLEXOR 14    // A0
#define EXTENSOR 15  // A1
#define LED 7

// Miscellaneous Initialization
int count_avg[2] = {0, 0};
const int emg_interval = 2; // In milisecs
int stepper_interval = 0;   // In milisecs

uint32_t current_time = 0;
uint32_t prev_emg_time = 0;
uint32_t prev_stepper_time = 0;

int motor_max = 1;
const int motor_min = 15;

const int one_sec_size = 500 / emg_interval; // One second of reading
uint32_t sum_one_sec[2] = {0, 0};
uint32_t avg_one_sec[2] = {0, 0};

int baseline[2] = {0, 0};
const int emg_max[2] = {115, 90};


// EMG initialization
const int contraction[2] = {FLEXOR, EXTENSOR};
float emg_val[2] = {0, 0};
float prev_emg_val[2] = {0, 0};
float filterd_emg[2] = {0, 0};
float prev_filtered[2] = {0, 0};
float alpha = 0.35;

// Stepper Motor initialization
int halfStepSeq[8][4] = 
{
  {1, 0, 0, 0}, // Only A is activated
  {1, 1, 0, 0}, // A and B
  {0, 1, 0, 0}, // Just B
  {0, 1, 1, 0}, // B and C 
  {0, 0, 1, 0}, // Just C
  {0, 0, 1, 1}, // C and D
  {0, 0, 0, 1}, // Just D
  {1, 0, 0, 1}  // A and D
};
int step_ind = 0; // current index in the stepping sequence

// Functions
void HPF (int index)
{
  filterd_emg[index] = alpha * (emg_val[index] - prev_emg_val[index]) + prev_filtered[index];
  prev_emg_val[index] = emg_val[index];
  prev_filtered[index] = filterd_emg[index];
}

int running_avg(float emg_val, int index)
{
  int avg = 0;
  sum_one_sec[index] += emg_val;
  count_avg[index]++;

  if (count_avg[index] > one_sec_size)
  {
    avg = sum_one_sec[index] / count_avg[index];
    sum_one_sec[index] -= (sum_one_sec[index] / one_sec_size);
    count_avg[index] = one_sec_size;
    return avg;
  }

  return 0;
}

// Main
void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(LED, LOW);
}

void loop() {
  static int which_contraction = 0;
  current_time = millis();

  // Read and filter EMG
  if (current_time - prev_emg_time >= emg_interval)
  {
    prev_emg_time = current_time;
    emg_val[0] = analogRead(contraction[0]);
    emg_val[1] = analogRead(contraction[1]);
    // HIGH PASS FILTER
    HPF(0);
    HPF(1);
    // Serial.print(filterd_emg[0]);
    // Serial.print(',');
    // Serial.println(filterd_emg[1]);
  }

  // EMG Collecting & Averaging
  avg_one_sec[0] = running_avg(filterd_emg[0], 0);
  avg_one_sec[1] = running_avg(filterd_emg[1], 1);
  // Serial.print(avg_one_sec[0]);
  // Serial.print(',');
  // Serial.println(avg_one_sec[1]);

  // Start Motor
  if (count_avg[0] == one_sec_size && count_avg[1] == one_sec_size)
  {
    digitalWrite(LED, HIGH); // Light indicator to begin
    which_contraction = max(avg_one_sec[0], avg_one_sec[1]);
    if (current_time - prev_stepper_time >= stepper_interval)
    {
      prev_stepper_time = current_time;
      digitalWrite(IN1, halfStepSeq[step_ind][0]);
      digitalWrite(IN2, halfStepSeq[step_ind][1]);
      digitalWrite(IN3, halfStepSeq[step_ind][2]);
      digitalWrite(IN4, halfStepSeq[step_ind][3]);
      step_ind = (which_contraction == avg_one_sec[0]) ? (step_ind + 1) % 8 : (step_ind - 1 + 8) % 8;
      stepper_interval = (which_contraction == avg_one_sec[0]) ? map(which_contraction, baseline[0], emg_max[0], motor_min, motor_max) : map(which_contraction, baseline[1], emg_max[1], motor_min, motor_max);
    }
  }
}
