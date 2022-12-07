# CPnP_Z test
4CompactPnP

The ADC of this PIC is designed to be full scale with the supply voltage. 2.048V reference voltage is used as ADC. From that value, the voltage at full scale is obtained by backward calculation.
```mermaid
graph TD;
    A[ADC_Ref_Voltage]-->B[ADC_LED_current];
    B---H[Display];
    H-->C{Determine the presence or absence of Laser};
    C--|laser_is_present|-->D[Display_Laser_temperature];
    D-->A;
    C--|No_Laser|-->E[ADC_pressure];
    E---F[Display_meter];
    F---G[Display];
    G-->A;
```
