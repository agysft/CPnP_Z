# CPnP_Z test
4CompactPnP

The ADC of this PIC is designed to be full scale with the supply voltage; the PIC has a reference voltage FVR=2.048V. Calculate that value backwards to find the Vdd voltage; ; once Vdd is known, we can AD convert the correct current and barometric pressure values. The temperature of the laser can be known via serial communication. When the laser is mounted, the temperature of the laser is displayed without pressure measurement.
```mermaid
<<<<<<< HEAD
graph TD;
    A[The ADC of this PIC is designed to be full scale with the supply voltage. 2.048V reference voltage is used as ADC. From that value, the voltage at full scale is obtained by backward calculation.]-->B[ADC the LED current.
Display it as a numerical value.];
    B-->C{Determine the presence or absence of Laser.};
    C--|If a laser is present|-->D;
    C--|No Laser|-->E;
    D[Display Laser temperature];
    E[ADC the pressure sensor output.]---F[Display it as a meter.];
    F---G[Display it numerically.];
    G-->A;
    D-->A;
=======
flowchart TD;
    A[ADC Ref Voltage]-->B[ADC LED current];
    B---H[Display];
    H-->C{{Determine the presence or absence of Laser}};
    C-->|laser is present|D[Display Laser temperature];
    D-->A;
    C-->|No Laser|E[ADC pressure];
    E---F[Display meter];
    F---G[Display];
    G-->A;
>>>>>>> origin/main
```
