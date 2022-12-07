# CPnP_Z test
4CompactPnP

The ADC of this PIC is designed to be full scale with the supply voltage. 2.048V reference voltage is used as ADC. From that value, the voltage at full scale is obtained by backward calculation.
```mermaid
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
```
