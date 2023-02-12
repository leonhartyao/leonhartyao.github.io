# FLYing maniPULATOR

## Summary

Conceptual and technology development for aerial manipulation with free fyling fully actuated multicopters.

## Description

Aim of the Flypulator project is the development of flight robots for manipulation of stationary objects. Possible application scenarios include cleaning tasks and vegetation trimming in hardly accessible places. The development is focused on fully actuated flight robots. Due to their high agility, they allow the use of lightweight manipulators with small degree of freedom. The most important fields of activity are the construction of the hardware, the development and implementation of the control system and a detailed modelling including aerodynamical simulations and experiments.[^1]

One of the lastest results was presented by Shuster et al. at IROS 2022 (see the [video](https://www.youtube.com/watch?v=jGJbtuPdbJg)).

<!-- [![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/jGJbtuPdbJg/0.jpg)](https://www.youtube.com/watch?v=jGJbtuPdbJg) -->

## Contributions

- Tilt-Hexarotor prototype
- Motion tracking system based on VIVE Tracker.
- Kinematic and dynamic models for the Tilt-Hexarotor
- Nonlinear controllability and control authority for the semi-fully-actuated aerial vehicle.
- Simulation framework in Simulink and Gazebo.
- controllers (feedback linearization, SMC, backstepping, etc.) and observers (KF, EKF) design and implemented in Matlab and ROS
- Visual inertial odometer (Stereo extension of [VINS-Mono](https://github.com/HKUST-Aerial-Robotics/VINS-Mono))

<figure markdown>
  ![Principle](../assets/images/UAVM.png){ width="500" }
  <figcaption>Design of the aerial manipulator</figcaption>
</figure>

<figure markdown>
  ![vio-hohe-str](../assets/images/vins-stereo-hohe-str.gif){ width="500" }
</figure>
<figure markdown>
  ![vio-hohe-str](../assets/images/google-map-vins.png){ width="500" }
  <figcaption>Outdoor demo of VINS-Stereo</figcaption>
</figure>
<!-- <img src="../../assets/images/vins-stereo-hohe-str.gif"  width="600">
<img src="../../assets/images/google-map-vins.png"  width="600"> -->

 Video: [Youtube](https://www.youtube.com/watch?v=XW3dV7o6F24&t=39s)

[^1]: https://tu-dresden.de/ing/maschinenwesen/ifkm/dmt/forschung/projekte/Flypulator
