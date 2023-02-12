# Small Linear and Planar Direct Drives

## Description

Linear direct drives with a simple single-phase structure allows short travel distances of up to approx. 25 mm, e.g., for automation and handling technology or small machine tools. Special drives with moving permanent magnets and iron-core stator windings enable large forces with low power losses and small installation space. A number of such compact, dynamic and cost-effective linear as well as a new type of planar direct drive were developed, built and tested. They are characterized by[^1]:

- accelerations up to 35 g,
- top executives up to over 100 N,
- integrated roller or spring guides,
- integrated incremental or absolute displacement sensors
- with resolutions of currently 0.16 to 1.25 µm,
- integrated flatness-based attitude control,
- sensorless force control,
- Control commands via EtherCAT, USB or RS-232.

<figure markdown>
  ![actors](https://www.ifte.de/forschung/elektromechanischerEntwurf/aktoren/actors_big.jpg){ width="500" }
  <figcaption>New small linear and planar direct drives with integrated position control [Copyright@ IFTE, TUD]</figcaption>
</figure>

## Contributions

Contributions are the measurement and control of the position of an electrodynamic planar actuator. An optical encoder and an interpolator are used for the positioning of
the Platen. The planar actuator consists of four linear motors arranged on the same surface. In this way the platen can be moved in X-direction, Y-direction or about the Z-axis in a limited angular range. For this reason, a multiple input multiple output controller with high dynamic performance is required. To fill the requirements, a path-following control with the robust pole placement is designed and simulated.

The novel x-y-$\varphi$ planar table has been prototyped: [video](https://www.ifte.de/forschung/elektromechanischerEntwurf/aktoren/Planartisch_vid1.mp4) [^1].

[^1]: https://www.ifte.de/forschung/elektromechanischerEntwurf/kleinantriebe.html
