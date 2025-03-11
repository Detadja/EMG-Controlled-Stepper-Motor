#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define FLEXOR 14    // A0
#define EXTENSOR 15  // A1
#define LED 7

int ind = 0;
const int emg_interval = 50;     // In milisecs
static int stepper_interval = 0; // In milisecs
uint32_t prev_emg_time = 0;
uint32_t prev_stepper_time = 0;
const int emg_min = 0;
static int motor_max = 1;
const int motor_min = 50;

const int one_sec_size = 1000 / emg_interval; // One second of reading
static int one_sec[one_sec_size][2] = {{0}};  // 0 = FLEXOR, 1 = EXTENSOR
static int avg_one_sec[2] = {0, 0};

const int baseline_size = one_sec_size * 5;    // Five seconds of reading
static int baseline[baseline_size][2] = {{0}}; // 0 = FLEXOR, 1 = EXTENSOR
bool baseline_read = false;
static int avg_baseline[2] = {0, 0};

const int max_emg_size = one_sec_size * 5;        // Five seconds of reading
static int max_emg_data[max_emg_size][2] = {{0}}; // 0 = FLEXOR, 1 = EXTENSOR
bool is_max = true;
static int emg_max[2] = {0, 0};

// EMG initialization
int contraction[2] = {FLEXOR, EXTENSOR};
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
  {0, 1, 0, 0}, // B and C 
  {0, 1, 1, 0}, // C and D 
  {0, 0, 1, 0}, // Just D
  {0, 0, 1, 1}, // D and end 
  {0, 0, 0, 1}, // just end 
  {1, 0, 0, 1}  // loop around
};
int step_ind = 0; // current index in the stepping sequence

// Functions
float avg_array(int emg_over_time[][2], int arr_size, bool is_flex)
{
  static int sum = 0;

  if (is_flex) // FLEX
  {
    for (int i = 0; i < arr_size; i++)
    {
      sum += emg_over_time[i][0];
    }
  }
  else // EXTEND
  {
    for (int i = 0; i < arr_size; i++)
    {
      sum += emg_over_time[i][1];
    }
  }

  return sum / arr_size;
}

int max_array(int max_emg[][2], int arr_size, bool is_flex)
{
  static int max = 0;
  if (is_flex) // FLEX
  {
    for (int i = 0; i < arr_size; i++)
    {
      max = (max_emg[i][0] >= max) ? max_emg[i][0] : max;
    }
  }
  else // EXTEND
  {
    for (int i = 0; i < arr_size; i++)
    {
      max = (max_emg[i][1] >= max) ? max_emg[i][1] : max;
    }
  }
  
  return max;
}

void HPF (float (&emg_val)[2], float (&prev_emg_val)[2], float (&filterd_emg)[2], float (&prev_filtered)[2], int index)
{
  filterd_emg[index] = alpha * (emg_val[index] - prev_emg_val[index]) + prev_filtered[index];
  prev_emg_val[index] = emg_val[index];
  prev_filtered[index] = filterd_emg[index];
}

void LED_indicator(bool is_max)
{
  if (is_max)
  {
    digitalWrite(LED, LOW);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
  }
  else 
  {
    digitalWrite(LED, HIGH);
  }
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
  uint32_t current_time = millis();

  // Read and filter EMG
  if (current_time - prev_emg_time >= emg_interval)
  {
    prev_emg_time = current_time;
    for (int i = 0; i < 2; i++) 
    {
      emg_val[i] = analogRead(contraction[i]);
      HPF(emg_val, prev_emg_val, filterd_emg, prev_filtered, i); // HIGH PASS FILTER
    }
    // Serial.print(filterd_emg[0]);
    // Serial.print(',');
    // Serial.println(filterd_emg[1]);
  }


  if (baseline_read && !is_max)
  {
    delay(1500);
    LED_indicator(is_max); // Light indicator to start full emg
    
    avg_baseline[0] = int(round(avg_array(baseline, baseline_size, true)));
    avg_baseline[1] = int(round(avg_array(baseline, baseline_size, false)));
    // Serial.print(avg_baseline[0]);
    // Serial.print(',');
    // Serial.println(avg_baseline[1]);
    emg_max[0] = max_array(max_emg_data, max_emg_size, true); 
    emg_max[1] = max_array(max_emg_data, max_emg_size, false); 
    // Serial.print(emg_max[0]);
    // Serial.print(',');
    // Serial.print(emg_max[1]);

    ind = (ind >= one_sec_size) ? 0 : ind;
    one_sec[ind][0] = filterd_emg[0];
    one_sec[ind][1] = filterd_emg[1];
    ind++;

    if (ind == one_sec_size) // Stepper Motor
    {
      avg_one_sec[0] = int(round(avg_array(one_sec, one_sec_size, true)));
      avg_one_sec[1] = int(round(avg_array(one_sec, one_sec_size, false)));
      // Serial.print(avg_one_sec[0]);
      // Serial.print(',');
      // Serial.print(avg_one_sec[1]);
      which_contraction = max(avg_one_sec[0], avg_one_sec[1]);
      // TODO: FIGURE OUT HOW TO GET stepper_interval TO STICK WITHOUT CHANGING THE DATA OVERTIME
      stepper_interval = (which_contraction == avg_one_sec[0]) ? map(which_contraction, emg_min, emg_max[0], motor_min, motor_max) : map(which_contraction, emg_min, emg_max[1], motor_min, motor_max);

      if (current_time - prev_stepper_time >= stepper_interval)
      {
        prev_stepper_time = current_time;
        digitalWrite(IN1, halfStepSeq[step_ind][0]);
        digitalWrite(IN2, halfStepSeq[step_ind][1]);
        digitalWrite(IN3, halfStepSeq[step_ind][2]);
        digitalWrite(IN4, halfStepSeq[step_ind][3]);
        step_ind = (contraction == avg_one_sec[0]) ? (step_ind + 1) % 8 : (step_ind - 1 + 8) % 8;
      }
    }
  }
  else if (baseline_read && is_max) // Gets Max EMG
  {
    ind = (ind >= max_emg_size) ? 0: ind;
    LED_indicator(is_max); // Light indicator to start recording max
    max_emg_data[ind][0] = filterd_emg[0];
    max_emg_data[ind][1] = filterd_emg[1];
    ind++;
    is_max = (ind == max_emg_size) ? true : false;
  }
  else if (!baseline_read) // Captures the baseline activity for 5 seconds
  {
    baseline[ind][0] = filterd_emg[0];
    baseline[ind][1] = filterd_emg[1];
    ind++;
    baseline_read = (ind == baseline_size) ? true : false;
  }
}
