# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---
## Introduction
 This project will implement a PID controller in C++ to maneuver the vehicle around the track.
 The simulator will provide a cross track error (CTE) and the velocity (mph) in order to compute the appropriate steering angle and throttle.
 The speed limit has been increased from 30 mph to 100 mph, and try to drive SAFELY as fast as possible.

## Code Description

  * I implemented the **PID algorithm** in file main.cpp for the steering angle (**pid_st**, line 36) and the throttle (**pid_th**, line 37) of car.  

  * There are 2 modes in the code  
    * Tune-mode: Running the track with an target speed (30km/h, 40km/h, 50km/h, 60km/h) using the finally appropriate hyperparameters (P, I, D coefficients). Below example shows the setting for 40km/h.  
          #./pid 40

    * Run-mode: Tunning the P, I, D coefficients of the steering angle and throttle using the twiddle method (need files of initial parameters: ***tune_steer.dat*** and ***tune_throt.dat***).
          #./pid 0          

## Rubric Discussion

 #### *1. Describe the effect each of the P, I, D components had in your implementation.*  
 *Student describes the effect of the P, I, D component of the PID algorithm in their implementation.*

  Each of the P, I, D component is described as follows:  

  * The **P (Proportional)** component is proportional to the current error (**Cross Track Error: CTE**; the distance of car to the lane center). If the car is far from the lane center,
    * It will steers back to the direction of the lane center.
    * The throttle will be decrease to reduce the speed.  


  * The **I (Integral)** component accounts for the historic cumulative errors of car to eliminate the residual error by adding the control effect. I manually tried the component, but it seemingly did the car going to unstable. So, I did not use the component in the project.

  * The **D (Differential)** component is the gradient component, estimates of the future trend error of car, based on its current rate of CTE. I found that it reduce the overshoot of car from the center lane. So, I manually increase the component parameter in the initialization for the large target speed of car, to make the car to be more stable.


#### *2. Describe how the final hyperparameters were chosen.*
*Student discusses how they chose the final hyperparameters (P, I, D coefficients). This could be have been done through manual tuning, twiddle, SGD, or something else, or a combination!*

I did the following steps to estimate the final hyperparameters (P, I, D coefficients) using the **src_tune**:  

  * **Tune Method**  (Using the tune-mode)  
    1) Manually setting the initial parameters for the target speed with the initial throttle in the "tune_steer.dat" and "tune_throt.dat"

        * speed 30km/h --> throttle:0.3
        * speed 40km/h --> throttle:0.4
        * speed 50km/h --> throttle:0.5
        * speed 60km/h --> throttle:0.6                

    2) Using the twiddle method to tune the P, I, D coefficients.Here, I run the tracks in about 12 circles (each circle has 2,000 steps, total 12 * 2,000 steps = 24,000 steeps). For each circle, the recommended parameters are written in the "tune_steer.dat" and "tune_throt.dat" for the steering angle and throttle, respectively.

    3) Referring the parameters, I manually decide the appropriate parameters, based on the average errors of each circle, and go the the step 1) for the next target speed running.

* **PID Results**  

    Following tables show the final hyperparameters for each target throttle test. See in details and others' parameters in the files "tune_steer.dat" and "tune_throt.dat". However, they should be assessed more to estimate the appropriate values in further study.  

   **Target Speed: 30km/h**

        Initial throttle: 0.3
        | Parameters  |    P   |   I    |    D   |
        | ----------  | ------ | ------ | ------ |
        | Steering    | 0.112  | 0.000  | 0.900  |
        | Throttle    | 0.109  | 0.000  |-0.005  |

    **Target Speed: 40km/h**

        Initial throttle: 0.4
        | Parameters  |    P   |   I    |    D   |
        | ----------  | ------ | ------ | ------ |
        | Steering    | 0.144  | 0.000  | 0.999  |
        | Throttle    | 0.216  | 0.000  | 0.001 |

  **Target Speed: 50km/h**

        Initial throttle: 0.5
        | Parameters  |    P   |   I    |    D   |
        | ----------  | ------ | ------ | ------ |
        | Steering    | 0.136  | 0.000  | 1.314  |
        | Throttle    | 0.179  | 0.000  |-0.009 |

  **Target Speed: 60km/h**

        Initial throttle: 0.6
        | Parameters  |    P   |   I    |    D   |
        | ----------  | ------ | ------ | ------ |
        | Steering    | 0.145  | 0.000  | 2.000  |
        | Throttle    | 0.221  | 0.000  |-0.009  |

## A Result Movie

  * Target Speed of 50km/h: [PID050.mp4](movie/PID050.mp4)

---
## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`.

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).
