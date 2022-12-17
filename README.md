# CPnP_Z test
4CompactPnP

The ADC of this PIC is designed to be full scale with the supply voltage; the PIC has a fixed voltage FVR=2.048V.
Calculate that value backwards to find the Vdd voltage; once Vdd is known, we can AD convert the correct LED current and pressure sensors values.
The temperature of the laser can be known via serial. When the laser is connected, the temperature of the laser is displayed without pressure measurement.
```mermaid
flowchart TD;
    I([START])---A;
    A[AD convert FVR]-->B[AD convert current of Upper-Light];
    B---H[Display numerical value];
    H---C{{With or without Laser?}};
    C-->|No Laser|E[AD convert of air pressure in pipe];
    E---F[[Display by meter]];
    F---G[Display numerical value];
    G-->A;
    C-->|Laser is present|D[Read Laser-temp via serial];
    D---D2[Display Laser temperature];
    D2---D3[Set Laser-marker Brightness];
    D3-->A;
```
