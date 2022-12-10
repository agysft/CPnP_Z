# CPnP_Z test
4CompactPnP

The ADC of this PIC is designed to be full scale with the supply voltage; the PIC has a fixed voltage FVR=2.048V.
Calculate that value backwards to find the Vdd voltage; once Vdd is known, we can AD convert the correct LED current and pressure sensors values.
The temperature of the laser can be known via serial. When the laser is connected, the temperature of the laser is displayed without pressure measurement.
```mermaid
flowchart TD;
    I([START])---A;
    A[ADC Ref Voltage]-->B[ADC LED current];
    B---H[Display];
    H-->C{{With or without Laser?}};
    C-->|No Laser|E[ADC pressure];
    E---F[Display meter];
    F---G[Display];
    G-->A;
    C-->|Laser is present|D[Read serial from Laser];
    D---D2[Display Laser temperature];
    D2---D3[Set Laser Marker Brightness];
    D3-->A;
```
