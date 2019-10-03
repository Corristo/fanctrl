#include <wiringPi.h>

#include <iostream>
#include <thread>

static constexpr int pin = 18;
static constexpr int range = 100;

// empirically determined lowest duty cycle to get fan to spin
static constexpr int min_duty_cycle = 70;

// temperature at which fan starts spinning, in Celsius
static constexpr int min_temp = 60;

// temperature at which fan runs at full speed, in Celsius
static constexpr int max_temp = 80;

int cpu_temperature() {
  // code adapted from https://raspberrypi.stackexchange.com/a/6979
  auto const temperature_file = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
  if (!temperature_file)
    std::exit(3);
  double T;
  fscanf(temperature_file, "%lf", &T);
  fclose(temperature_file);
  return static_cast<int>(T)/1000;
}

int main() {
  using namespace std::chrono_literals;

  if(wiringPiSetupGpio() < 0) {
    std::exit(1);
  }

  pinMode(pin, PWM_OUTPUT);
  pwmSetMode(PWM_MODE_BAL);
  pwmSetClock(100);
  pwmSetRange(range);

  // start at full speed
  pwmWrite(pin, range);

  while(true) {
    // only change fan speed every 5s
    std::this_thread::sleep_for(5s);
    auto const temp = cpu_temperature();

    if (temp < min_temp) {
      // turn fan off
      pwmWrite(pin, 0);
    } else if (temp > max_temp) {
      // run at full speed
      pwmWrite(pin, range);
    } else {
      auto const speed_perc = (temp - min_temp) * 1.0/(max_temp - min_temp);
      auto const duty_cycle = min_duty_cycle + static_cast<int>(speed_perc * (range - min_duty_cycle));
      pwmWrite(pin, duty_cycle);
    }
  }
}
