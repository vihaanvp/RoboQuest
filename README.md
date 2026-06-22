# RoboQuest — Code that I used

This repository contains multiple Arduino sketches developed for various rounds and objectives of a robotics competition by Dr. Homibhabha Foundation named RoboQuest. Each project implements a different behavior — from basic line following to obstacle avoidance, parking, U-turns, and multi-stage state machines.

> ⚠️ Pinout, delay constants, and motor polarity may differ between sketches depending on the hardware setup used at the time. Adjust as needed for your own machine.

## Technology Utilized

- **Language:** Embedded C++
- **IDE:** Arduino IDE
- **Hardware:** Arduino Uno / Nano, IR sensors (2–4), HC-SR04 ultrasonic sensor, L293D motor driver, LDR + LED

---

## Sketch Categories & Feature Comparison

---

### 2-IR Line Followers

| Feature | `Normal_Line_Follower` | `LF_with_Headlight` |
|---|---|---|
| **Line following** | Standard 2-IR (forward / turn / stop) | Standard 2-IR (forward / turn / stop) |
| **Headlight (LDR + LED)** | ❌ None | ✅ LED turns on when `analogRead(A0) < 450` (dark / tunnel) |
| **Speed control** | Full speed only | Full speed only |
| **Motor interface** | Direct L293D pin control (LOW = forward) | Direct L293D pin control (HIGH = forward) |
| **Extra hardware** | None | LDR on A0, LED on pin 6 |
| **Code quality** | ✅ Clean, minimal, well-structured | ✅ Clean, minimal |
| **Best for** | Pure, simple line following | Line following through dark tunnels with visual feedback |

---

### Line Followers with Ultrasonic Obstacle Avoidance

| Feature | `Obstacle_Avoiding` | `Line_Follower_Obstacle_Sweep` | `Line_Follower_Ultrasonic` |
|---|---|---|---|
| **Avoidance style** | ✅ Parallel pass — turn right, go alongside obstacle, turn left back to line | ✅ Sweep — turn around obstacle, actively scan for the line, re-acquire | ❌ Rigid 14-step pre-set sequence (~13s, blocking) |
| **Non-blocking** | ✅ Yes (millis-based state timers) | ✅ Yes (millis-based, stays responsive throughout) | ❌ No (relies on `delay()`, freezes robot) |
| **Line re-acquisition** | ✅ Stable-line detection with min/max timeout (250ms–1800ms) | ✅ Sweep scan + 120ms confirmation debounce | ❌ None — assumes line is still ahead |
| **Number of obstacles** | Unlimited (continuous detection) | Unlimited (continuous detection) | Hard-coded for exactly 2 obstacles |
| **State machine** | 7 states (FOLLOW_LINE → AVOID_STOP → TURN_OUT → GO_PARALLEL → PASS → TURN_IN → REACQUIRE) | 2 states with 14-step avoidance sub-machine (FOLLOW_LINE / AVOIDING) | 5 states (FOLLOW_LINE → AVOID → FOLLOW_AGAIN → AVOID_2 → FINAL_FOLLOW) |
| **Extra features** | Configurable distance thresholds and timing constants | Configurable timing, sweep direction toggle | LDR + LED headlight, Serial debug output |
| **Line polarity** | Standard (LOW = on line) | Standard (LOW = on line) | ⚠️ Inverted (HIGH = on line) |
| **Motor convention** | LOW = forward | LOW = forward | HIGH = forward |
| **Code quality** | ✅ Excellent — clean, well-organized, well-commented | ✅ Excellent — clean, organized, well-commented state machine | ⚠️ Fair — contains duplicate code patterns, large commented blocks, redundant sensor reads |
| **Best for** | Reliable obstacle navigation that actually works | Courses where the line may be off-angle after avoiding an obstacle | Fixed courses with at most 2 known obstacles where blocking is acceptable |

---

### Obstacle Detection via IR (Non-Ultrasonic)

| Feature | `Line_Follower_Obstacle_IR` |
|---|---|
| **Obstacle sensor** | IR proximity pair (AVOID_LEFT / AVOID_RIGHT) |
| **Avoidance behavior** | Stop → wait 500ms → move backward & turn → resume |
| **Non-blocking** | ❌ No (relies on `delay()`) |
| **Line re-acquisition** | ❌ None — just resumes forward |
| **Speed control** | ✅ PWM (~20% duty cycle via `analogWrite`) |
| **Line polarity** | Standard (LOW = on line) |
| **Motor convention** | LOW = forward |
| **Code quality** | ⚠️ Fair — misleading comments ("backup and turn" but both motors go HIGH/HIGH which is brake), redundant obstacle logic (duplicate conditions in the `if`) |
| **Best for** | Quick stop-and-go obstacle detection with PWM speed control and no need for actual navigation around objects |

---

### Special Maneuvers

| Feature | `UTURN` | `Parking` | `OLD_Obstacle_Avoider_Testing` |
|---|---|---|---|
| **Primary action** | Line follow → U-turn on double line detect → follow back | Ultrasonic spot finding → turn → line follow into parking spot | Skeleton/test code with `AvoidObject()` defined but `loop()` is **empty** (non-functional) |
| **Sensor used** | 2 IR + ultrasonic (unused) + LDR | Ultrasonic + 2 IR | Ultrasonic + 2 IR |
| **State machine** | 4 states (FOLLOW_LINE → U_TURN → FOLLOW_LINE_BACK → STOP) | 3 states (FIND_SPOT → TURN → PARK) | ❌ None (empty loop) |
| **Extra features** | LDR + LED headlight (`analogRead(A0) < 450`) | — | — |
| **Motor convention** | HIGH = forward | HIGH = forward | HIGH = forward |
| **Code quality** | ✅ Good, simple, clear | ⚠️ Functional but rough — unused variables, no distance check in `loop()` | ⚠️ Incomplete — `AvoidObject()` is defined but never called |
| **Best for** | Tracks with a U-turn section | Courses with a designated parking zone | Reference / starting point for building an obstacle avoider |

---

### Multi-Stage / Objective-Based

| Feature | `State_Round / Objective_1` |
|---|---|
| **Objective** | Complete a multi-stage course with timed stops, obstacle detection, and bottle-zone navigation |
| **State machine** | 10 states: FOLLOW_LINE → STOP_2s → FOLLOW_LINE_2 → STOP_3s → FOLLOW_LINE_3 (with obstacle check ≤17cm) → STOP_3s_2 → FOLLOW_LINE_4 → FINAL_STOP_1s → BOTTLES_PRE_CIRCLE → BOTTLES_IN_CIRCLE |
| **Non-blocking** | ❌ No (uses `delay()` for timed stops) |
| **Sensor used** | 2 IR + ultrasonic for obstacle detection |
| **Line polarity** | Standard (LOW = on line) |
| **Motor convention** | LOW = forward (stop = both LOW) |
| **Serial debug** | ✅ Prints distance during obstacle-check state |
| **Code quality** | ⚠️ Fair — some repeated line-following code across states, could be refactored into a shared function; mixed concerns (ultrasonic reading done in `loop()` before the switch but only used in one state) |
| **Best for** | Competition rounds with defined sequential objectives, timed pauses, and bottle-handling zones |

---

## Author
[vihaanvp](https://github.com/vihaanvp/)